package com.example.myles.secondapp;

import android.os.Looper;
import android.os.Message;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.view.View;
import android.widget.Button;

import java.lang.reflect.Array;
import java.lang.reflect.Method;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.UUID;
import java.util.Vector;

import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;
import android.content.Context;
import android.content.Intent;
import android.os.Handler;


public class MainActivity extends ActionBarActivity {

    ConnectThread myThread;

    BluetoothAdapter mBluetoothAdapter;
    private BluetoothDevice rnBoard;
    CharSequence REFRESH = "Refreshed!";
    CharSequence delA = "A Deleted";
    CharSequence delB = "B Deleted";
    CharSequence consA = "Consolidated on A";
    CharSequence consB = "Consolidated on B";
    CharSequence swapped = "Swapped";
    CharSequence BlueToothAlreadyOn = "Bluetooth Already On";
    int duration = Toast.LENGTH_SHORT;
    byte[] DelA_ByteArray = "dea\n".getBytes();
    byte[] DelB_ByteArray = "deb\n".getBytes();
    byte[] ConA_ByteArray = "coa\n".getBytes();
    byte[] ConB_ByteArray = "cob\n".getBytes();
    byte[] Swap_ByteArray = "swa\n".getBytes();
    byte[] Ref_ByteArray = "ref\n".getBytes();

    Integer tag_a = 0, tag_b = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        int REQUEST_ENABLE_BT = 1;
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (mBluetoothAdapter != null) {
            if (mBluetoothAdapter.isEnabled()) {
                Context context = getApplicationContext();
                Toast toast = Toast.makeText(context, BlueToothAlreadyOn, duration);
                toast.show();
            } else {
                Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
            }

        } else {
            //no bluetooth device, not really a worry for me
            Context context = getApplicationContext();
            Toast toast = Toast.makeText(context, "No Bluetooth Device Detected", duration);
            toast.show();
        }
        while(myThread == null) {
            Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();
            boolean bluetoothExists = false;
            for (BluetoothDevice device : pairedDevices) {

                String name = device.getName();
                if ( name.contains("RFID") ) {
                    rnBoard = device;
                    bluetoothExists = true;
                }

            }
            if (bluetoothExists) {
                myThread = new ConnectThread(rnBoard);
                myThread.start();
                Toast.makeText(getApplicationContext(), "Paired with RFID reader", duration).show();
            }
        }
    }

    //Function called when refresh button is pressed
    public void refresh(View view) {
        myThread.write(Ref_ByteArray);
        Context context = getApplicationContext();

        TextView aText = (TextView)findViewById(R.id.TextViewA);
        aText.setText(tag_a.toString());

        TextView bText = (TextView)findViewById(R.id.TextViewB);
        bText.setText(tag_b.toString());


        Toast toast = Toast.makeText(context, REFRESH, duration);
        toast.show();
    }

    //Function called when delete A button is pressed
    public void deleteA(View view) {
        myThread.write(DelA_ByteArray);
        Context context = getApplicationContext();
        Toast toast = Toast.makeText(context, delA, duration);
        toast.show();
    }

    //Function called when delete B button is pressed
    public void deleteB(View view) {
        myThread.write(DelB_ByteArray);
        Context context = getApplicationContext();
        Toast toast = Toast.makeText(context, delB, duration);
        toast.show();
    }

    //Function called when delete B button is pressed
    public void conA(View view) {
        myThread.write(ConA_ByteArray);
        Context context = getApplicationContext();
        Toast toast = Toast.makeText(context, consA, duration);
        toast.show();
    }

    public void conB(View view) {
        myThread.write(ConB_ByteArray);
        Context context = getApplicationContext();
        Toast toast = Toast.makeText(context, consB, duration);
        toast.show();
    }

    public void swap(View view) {
        myThread.write(Swap_ByteArray);
        Context context = getApplicationContext();
        Toast toast = Toast.makeText(context, swapped, duration);
        toast.show();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    Handler mHandler = new Handler(){
        @Override
        public void handleMessage(Message msg){
            tag_a = myThread.data_a;
            tag_b = myThread.data_b;
        }
    };

    private class ConnectThread extends Thread {

        private OutputStream mmoutStream;
        private InputStream mminStream;
        private  BluetoothSocket mmSocket;
        private final BluetoothDevice mmDevice;
        public UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

        List<Character> bits_received = new ArrayList<Character>();
        int data_a, data_b;

        public ConnectThread(BluetoothDevice device) {
            // Use a temporary object that is later assigned to mmSocket,
            // because mmSocket is final
            BluetoothSocket tmp = null;
            mmDevice = device;

            // Get a BluetoothSocket to connect with the given BluetoothDevice
            try {
                // MY_UUID is the app's UUID string, also used by the server code
                tmp = device.createRfcommSocketToServiceRecord(MY_UUID);
            } catch (IOException e) {
            }
            mmSocket = tmp;
        }
        public void run() {
            // Cancel discovery because it will slow down the connection
            // mBluetoothAdapter.cancelDiscovery();

            try {
                // Connect the device through the socket. This will block
                // until it succeeds or throws an exception
                mmSocket.connect();
            } catch (IOException connectException) {
                // Unable to connect; close the socket and get out
                try {
                    mmSocket.close();
                } catch (IOException closeException) { }
                return;
            }

            // Do work to manage the connection (in a separate thread)
            manageConnectedSocket(mmSocket);
        }

        private void manageConnectedSocket(BluetoothSocket socket)
        {
            mmSocket = socket;
            InputStream tmpIn = null;
            OutputStream tmpOut = null;
            // Some garbage with the socket.

            try {
                tmpIn = socket.getInputStream();
                tmpOut = socket.getOutputStream();
            } catch (IOException e) { }

            mminStream = tmpIn;
            mmoutStream = tmpOut;

            inStreamListen();
        }

        public void inStreamListen(){
            byte[] buffer = new byte[1024];  // buffer store for the stream
            int readBytes;  // bytes read from input stream/from read

            while (true) {
                try {
                    // Read from the InputStream
                    readBytes = mminStream.read(buffer);
                    // parse data from buffer

                    for(int i = 0; i < readBytes; i++)
                    {
                        char charRead = (char)buffer[i];
                        bits_received.add(charRead);
                    }

                    if(readBytes == 0) break;

                    if(bits_received.size() >= 8 && bits_received.get(0) == 'd' && bits_received.get(4) == 'd') {
                        data_a = (int)bits_received.get(2);
                        data_b = (int)bits_received.get(6);
                        mHandler.handleMessage(null);
                        bits_received.clear();
                    } else if (bits_received.size() > 8) {
                        bits_received.clear();
                    }
                }
                catch (IOException e) {
                    break;
                }
            }
        }

        public void write(byte[] bytes) {
            try {
                mmoutStream.write(bytes);
            } catch (IOException e) { }
        }

        public void cancel() {
            try {
                mmSocket.close();
            } catch (IOException e) { }
        }
    }
}
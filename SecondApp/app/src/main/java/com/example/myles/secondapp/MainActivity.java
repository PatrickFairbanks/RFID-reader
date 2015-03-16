package com.example.myles.secondapp;

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
import java.util.Set;
import java.util.UUID;

import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Toast;
import android.content.Context;
import android.content.Intent;


public class MainActivity extends ActionBarActivity {

    private OutputStream mmoutStream;
    private InputStream mminStream;
    BluetoothAdapter mBluetoothAdapter;
    CharSequence REFRESH = "Refreshed!";
    CharSequence delA = "A Deleted";
    CharSequence delB = "B Deleted";
    CharSequence consA = "Consolidated on A";
    CharSequence consB = "Consolidated on B";
    CharSequence swapped = "Swapped";
    CharSequence BlueToothAlreadyOn = "Bluetooth Already On";
    int duration = Toast.LENGTH_SHORT;

    //need to write a set of byte sequences for the refresh, delete, consolidate and swap to call
    //to write to output streams


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
    }

    //Function called when refresh button is pressed
    public void refresh(View view) {
        Context context = getApplicationContext();
        Toast toast = Toast.makeText(context, REFRESH, duration);
        toast.show();
    }

    //Function called when delete A button is pressed
    public void deleteA(View view) {
        Context context = getApplicationContext();
        Toast toast = Toast.makeText(context, delA, duration);
        toast.show();
    }

    //Function called when delete B button is pressed
    public void deleteB(View view) {
        Context context = getApplicationContext();
        Toast toast = Toast.makeText(context, delB, duration);
        toast.show();
    }

    //Function called when delete B button is pressed
    public void conA(View view) {
        Context context = getApplicationContext();
        Toast toast = Toast.makeText(context, consA, duration);
        toast.show();
    }

    public void conB(View view) {
        Context context = getApplicationContext();
        Toast toast = Toast.makeText(context, consB, duration);
        toast.show();
    }

    public void swap(View view) {
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

    private class ConnectThread extends Thread {
        private final BluetoothSocket mmSocket;
        private final BluetoothDevice mmDevice;
        public UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

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
            mBluetoothAdapter.cancelDiscovery();

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
    }

    private void manageConnectedSocket(BluetoothSocket mmSocket)
    {
        InputStream tmpIn = null;
        OutputStream tmpOut = null;
        // Some garbage with the socket.

        try {
            tmpIn = socket.getInputStream();
            tmpOut = socket.getOutputStream();
        } catch (IOException e) { }

        mminStream = tmpIn;
        mmoutStream = tmpOut;
    }

    public void inStreamListen(){
        byte[] buffer = new byte[1024];  // buffer store for the stream
        int readBytes;  // bytes read from input stream/from read

        while (true) {
            try {
                // Read from the InputStream
                readBytes = mminStream.read(buffer);
                // Send the obtained bytes to the UI activity
                mHandler.obtainMessage(MESSAGE_READ, readBytes, -1, buffer)
                        .sendToTarget();
            } catch (IOException e) {
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

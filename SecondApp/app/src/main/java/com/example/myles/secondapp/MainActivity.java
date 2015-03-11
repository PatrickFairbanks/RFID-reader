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
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Toast;
import android.content.Context;
import android.content.Intent;


public class MainActivity extends ActionBarActivity {

    private OutputStream outputStream;
    private InputStream inStream;
    BluetoothAdapter mBluetoothAdapter;
    CharSequence REFRESH = "Refreshed!";
    CharSequence delA = "A Deleted";
    CharSequence delB = "B Deleted";
    CharSequence consA = "Consolidated on A";
    CharSequence consB = "Consolidated on B";
    CharSequence swapped = "Swapped";
    CharSequence BlueToothAlreadyOn = "Bluetooth Already On";
    int duration = Toast.LENGTH_SHORT;





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
    public void refresh(View view){
        Context context = getApplicationContext();
        Toast toast = Toast.makeText(context, REFRESH, duration);
        toast.show();
    }

    //Function called when delete A button is pressed
    public void deleteA(View view){
        Context context = getApplicationContext();
        Toast toast = Toast.makeText(context, delA, duration);
        toast.show();
    }

    //Function called when delete B button is pressed
    public void deleteB(View view){
        Context context = getApplicationContext();
        Toast toast = Toast.makeText(context, delB, duration);
        toast.show();
    }

    //Function called when delete B button is pressed
    public void conA(View view){
        Context context = getApplicationContext();
        Toast toast = Toast.makeText(context, consA, duration);
        toast.show();
    }

    public void conB(View view){
        Context context = getApplicationContext();
        Toast toast = Toast.makeText(context, consB, duration);
        toast.show();
    }

    public void swap(View view){
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

}


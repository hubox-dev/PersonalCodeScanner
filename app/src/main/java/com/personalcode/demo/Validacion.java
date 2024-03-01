package com.personalcode.demo;

import android.app.Activity;
import android.util.Base64;
import android.util.Log;
import android.view.ViewGroup;
import android.widget.Toast;

import com.personalcode.PersonalCodeInterface;
import com.personalcode.PersonalCodeScanner;

public class Validacion {
    public PersonalCodeScanner personalCodeScanner;

    public Validacion(Activity actividad) throws Exception {
        if (personalCodeScanner==null) {
            personalCodeScanner = PersonalCodeScanner.getInstance(actividad, personalCodeInterface);
        }
    }

    public final PersonalCodeInterface personalCodeInterface = new PersonalCodeInterface() {
        @Override
        public boolean onCaptured(Activity activity, String qr0, String qr1) {
            activity.runOnUiThread(()-> Toast.makeText(activity, "QRs Escaneados", Toast.LENGTH_LONG).show());
            Log.d("SCANNING", "QR " + getIndex(qr0) + ": " + qr0);
            Log.d("SCANNING", "QR " + getIndex(qr1) + ": " + qr1);
            return true;
        }

        @Override
        public void onOpened(Activity activity, ViewGroup rootView) {
            
        }

        @Override
        public boolean onClose(Activity activity, int error, String description) {
            if (error<100) {
                activity.runOnUiThread(()-> Toast.makeText(activity, description, Toast.LENGTH_LONG).show());

                Log.e("SCANNING", description);
                //context.startActivity(new Intent(context, Principal.class));
            }
            return true;
        }
    };

    int getIndex(String qrBase64) {
        return Base64.decode(qrBase64, Base64.DEFAULT)[1];
    }
}

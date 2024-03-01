package com.personalcode.demo;

import android.Manifest;
import android.app.Activity;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.widget.Button;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

public class Principal extends AppCompatActivity {

    private Validacion validacion;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.layout_principal);

        try {
            validacion = new Validacion(this);
        } catch (Exception e) {
            Toast.makeText(this, "SDK initialization error: " + e.getMessage(), Toast.LENGTH_LONG).show();
        }

        Button btnScan = findViewById(R.id.btnScan);
        btnScan.setOnClickListener(v -> {
            int rc = ActivityCompat.checkSelfPermission(Principal.this, Manifest.permission.CAMERA);
            if (rc == PackageManager.PERMISSION_GRANTED) {
                permisosRecebidos();
            } else {
                pedirPermisoDeCamara(Principal.this);
            }
        });
    }

    private void permisosRecebidos() {
        if (validacion!=null) {
            validacion.personalCodeScanner.capture(this, false);
        } else {
            Toast.makeText(this, "SDK not initialized", Toast.LENGTH_LONG).show();
        }
    }

    public final int CODIGO_PERMISO_CAMARA = 2;
    public void pedirPermisoDeCamara(Activity actividad) {
        final String[] permissions = new String[]{Manifest.permission.CAMERA};
        ActivityCompat.requestPermissions(actividad, permissions, CODIGO_PERMISO_CAMARA);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, String[] permissions, int[] grantResults) {
        if (requestCode != CODIGO_PERMISO_CAMARA) {
            super.onRequestPermissionsResult(requestCode, permissions, grantResults);
            return;
        }

        if (grantResults.length != 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
            permisosRecebidos();
        }
    }
}

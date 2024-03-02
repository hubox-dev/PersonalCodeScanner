# PersonalCodeScanner

Para la implementación del SDK en Android, se necesita solicitar una licencia de integración. Sin la licencia no será posible ejecutar el SDK. La licencia es puesta en el archivo Gradle del módulo **app** y se compilan junto con la aplicación.

### Para que generemos la licencia necesitaremos que nos envíe una solicitud conteniendo el applicationId de la aplicación al siguiente contacto:
Teléfono: + (52) 55 8596 5748 / 49
+ (52) 55 6363 4776 / 77

Mail: info@hubox.com

https://personalcode.com/

## Para la integración del SDK se necesitará lo siguiente:

### Carpeta android/app/libs

#### Se copia el archivo PersonalCodeScanner.aar

### Configuración Gradle:

```
android {
    defaultConfig {
        buildConfigField "String", "license", "\"PLACE LICENSE HERE\""
    }
}
```

Agregamos el SDK a las dependencias:

```
dependencies {
    implementation 'com.google.android.material:material:1.7.0'
    implementation 'androidx.constraintlayout:constraintlayout:2.1.4'

    //SDK implementation:
    implementation files("libs/PersonalCodeScanner.aar")
}
```

### Clases Java o Kotlin (Basarse en en ejemplo de implementación):
- La clase Validacion.java contiene un ejemplo de implementación del SDK para escanear los QRs de nuestras credenciales

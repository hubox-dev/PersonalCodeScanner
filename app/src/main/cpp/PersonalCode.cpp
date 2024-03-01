#include <jni.h>
#include <string>
#include <chilkat9_5_0/CkGlobal.h>
#include <chilkat9_5_0/CkEcc.h>
#include <chilkat9_5_0/CkCrypt2.h>
#include <chilkat9_5_0/CkPublicKey.h>

using namespace std;

int strLen(const char* cadena) {
    int longitud = 0;
    while (cadena[longitud] != '\0') {
        longitud++;
    }
    return longitud;
}

bool strEquals(const char* cadena1, const char* cadena2) {
    while (*cadena1 != '\0' && *cadena1 == *cadena2) {
        cadena1++;
        cadena2++;
    }

    return (*cadena1 == *cadena2);
}

string jstr2str(JNIEnv *env, jstring jStr) {
    if (env->ExceptionCheck())
        env->ExceptionClear();
    if (!jStr)
        return "";

    jboolean isCopy;
    const char *convertedValue = (env)->GetStringUTFChars(jStr, &isCopy);
    const jsize len = env->GetStringUTFLength(jStr);
    string ret = string(convertedValue, len);

    delete[] convertedValue;
    convertedValue = nullptr;

    return ret;
}

int splitLength;
jstring* jstrSplit(JNIEnv *env, jstring jstr, jstring rejex) {
    if (env->ExceptionCheck())
        env->ExceptionClear();
    jclass String = env->GetObjectClass(jstr);
    jmethodID split = env->GetMethodID(String, "split", "(Ljava/lang/String;)[Ljava/lang/String;");
    auto jresult = (jobjectArray) env->CallObjectMethod(jstr, split, rejex);

    jclass Array = env->FindClass("java/lang/reflect/Array");
    jmethodID getLength = env->GetStaticMethodID(Array, "getLength", "(Ljava/lang/Object;)I");
    splitLength = env->CallStaticIntMethod(Array, getLength, jresult);

    auto *result = new jstring[splitLength];
    for (int i=0; i<splitLength; i++) {
        result[i] = (jstring) env->GetObjectArrayElement(jresult, i);
    }

    env->DeleteLocalRef(String);
    String = nullptr;
    env->DeleteLocalRef(jresult);
    jresult = nullptr;
    env->DeleteLocalRef(Array);
    Array = nullptr;

    return result;
}

jstring jstr2LowerCase(JNIEnv *env, jstring jstr) {
    if (env->ExceptionCheck())
        env->ExceptionClear();
    jclass String = env->GetObjectClass(jstr);
    jmethodID toLowerCase = env->GetMethodID(String, "toLowerCase", "()Ljava/lang/String;");
    jstr = (jstring) env->CallObjectMethod(jstr, toLowerCase);

    env->DeleteLocalRef(String);
    String = nullptr;

    return jstr;
}

string str2LowerCase(JNIEnv *env, const string& str) {
    if (env->ExceptionCheck())
        env->ExceptionClear();
    jstring jstr = env->NewStringUTF(str.c_str());
    jstring jresult = jstr2LowerCase(env, jstr);
    string result = jstr2str(env, jresult);

    env->DeleteLocalRef(jstr);
    jstr = nullptr;
    env->DeleteLocalRef(jresult);
    jresult = nullptr;

    return result;
}

string* strSplit(JNIEnv *env, const string& str, const string& rejex) {
    if (env->ExceptionCheck())
        env->ExceptionClear();
    jstring jstr = env->NewStringUTF(str.c_str());
    jstring jrejex = env->NewStringUTF(rejex.c_str());
    jstring* jresult = jstrSplit(env, jstr, jrejex);

    auto *result = new string[splitLength];
    for (int i=0; i<splitLength; i++) {
        result[i] = jstr2str(env, jresult[i]);
    }

    env->DeleteLocalRef(jstr);
    jstr = nullptr;
    env->DeleteLocalRef(jrejex);
    jrejex = nullptr;
    delete[] jresult;
    jresult = nullptr;

    return result;
}

jstring jstrReplace(JNIEnv *env, jstring jstr, jstring oldChar, jstring newChar) {
    if (env->ExceptionCheck())
        env->ExceptionClear();
    jclass String = env->FindClass("java/lang/String");
    jmethodID replace = env->GetMethodID(String, "replace", "(Ljava/lang/CharSequence;Ljava/lang/CharSequence;)Ljava/lang/String;");

    env->DeleteLocalRef(String);
    String = nullptr;

    return (jstring) env->CallObjectMethod(jstr, replace, oldChar, newChar);
}

string strReplace(JNIEnv *env, const string& str, const string& oldChar, const string& newChar) {
    if (env->ExceptionCheck())
        env->ExceptionClear();
    return jstr2str(env, jstrReplace(env, env->NewStringUTF(str.c_str()), env->NewStringUTF(oldChar.c_str()), env->NewStringUTF(newChar.c_str())));
}

const jint str2IntERROR = -100000;
jint str2Int(JNIEnv *env, string str) {
    if (env->ExceptionCheck())
        env->ExceptionClear();
    jclass Integer = env->FindClass("java/lang/Integer");
    jmethodID parseInt = env->GetStaticMethodID(Integer, "parseInt", "(Ljava/lang/String;)I");
    jint res = env->CallStaticIntMethod(Integer, parseInt, env->NewStringUTF(str.c_str()));

    if (env->ExceptionCheck()) {
        env->ExceptionClear();
        res = str2IntERROR;
    }
    env->DeleteLocalRef(Integer);
    Integer = nullptr;
    return res;
}

string getToday(JNIEnv *env) {
    jclass Date = env->FindClass("java/util/Date");
    jmethodID initDate = env->GetMethodID(Date, "<init>", "()V");
    jobject date = env->NewObject(Date, initDate);

    jclass SDF = env->FindClass("java/text/SimpleDateFormat");
    jmethodID sdfInit = env->GetMethodID(SDF, "<init>", "(Ljava/lang/String;)V");
    jmethodID format = env->GetMethodID(SDF, "format", "(Ljava/util/Date;)Ljava/lang/String;");
    jobject sdf = env->NewObject(SDF, sdfInit, env->NewStringUTF("yyyyMMdd"));

    string sDate = jstr2str(env, (jstring) env->CallObjectMethod(sdf, format, date));

    env->DeleteLocalRef(Date);
    env->DeleteLocalRef(date);
    env->DeleteLocalRef(SDF);
    env->DeleteLocalRef(sdf);

    return sDate;
}

jclass getBuildConfig(JNIEnv *env, jobject context) {
    jclass ParScan = env->FindClass("com/personalcode/PersonalCodeScanner");
    jfieldID fAppPackage = env->GetStaticFieldID(ParScan, "currentActivity", "Landroid/app/Activity;");
    jobject appPack = env->GetStaticObjectField(ParScan, fAppPackage);

    env->DeleteLocalRef(ParScan);

    jclass Activity = env->FindClass("android/app/Activity");
    jmethodID getLocalClassName = env->GetMethodID(Activity, "getLocalClassName", "()Ljava/lang/String;");
    auto jpkgN = (jstring) env->CallObjectMethod(appPack, getLocalClassName);

    env->DeleteLocalRef(Activity);
    env->DeleteLocalRef(appPack);

    jclass String = env->FindClass("java/lang/String");
    jmethodID lastIndexOf = env->GetMethodID(String, "lastIndexOf", "(Ljava/lang/String;)I");
    jint ind = env->CallIntMethod(jpkgN, lastIndexOf, env->NewStringUTF("."));
    string pkgN = jstr2str(env, jpkgN);

    if (ind>0) {
        jmethodID substring = env->GetMethodID(String, "substring", "(II)Ljava/lang/String;");
        pkgN = jstr2str(env, (jstring) env->CallObjectMethod(jpkgN, substring, 0, ind));
    } else {
        jclass ContextWrapper = env->FindClass("android/content/ContextWrapper");
        jmethodID getPackageName = env->GetMethodID(ContextWrapper, "getPackageName", "()Ljava/lang/String;");
        pkgN = jstr2str(env, (jstring) env->CallObjectMethod(context, getPackageName));

        env->DeleteLocalRef(ContextWrapper);
    }

    /*if (pkgN=="com.personalcode") {
        return nullptr;
    }*/

    env->DeleteLocalRef(String);
    env->DeleteLocalRef(jpkgN);

    if (!pkgN.empty()) {
        string pbcn = pkgN + ".BuildConfig";
        pbcn = strReplace(env, pbcn, ".", "/");

        auto bConf = env->FindClass(pbcn.c_str());

        if (env->ExceptionCheck())
            env->ExceptionClear();

        return bConf;
    }

    if (env->ExceptionCheck())
        env->ExceptionClear();

    return nullptr;
}

string getBundleID(JNIEnv *env, jobject context) {
    jclass bConfig = getBuildConfig(env, context);
    if (bConfig== nullptr) {
        return "";
    }

    jfieldID varID = env->GetStaticFieldID(bConfig, "APPLICATION_ID", "Ljava/lang/String;");
    string currentAppB = jstr2str(env, (jstring) env->GetStaticObjectField(bConfig, varID));

    env->DeleteLocalRef(bConfig);
    return currentAppB;
}

bool validLic = false;
string mFullLic;

const string nDiv = "<linha>";
string getFullLicense(JNIEnv *env, jobject context) {
    if (validLic) {
        return mFullLic;
    }

    jclass bConfig = getBuildConfig(env, context);
    if (bConfig== nullptr) {
        return "";
    }

    jfieldID licenseID = env->GetStaticFieldID(bConfig, "license", "Ljava/lang/String;");
    if (env->ExceptionCheck())
        env->ExceptionClear();

    if (licenseID != nullptr) {
        mFullLic = jstr2str(env, (jstring) env->GetStaticObjectField(bConfig, licenseID));
        mFullLic = strReplace(env, mFullLic, "|", nDiv);
    }

    env->DeleteLocalRef(bConfig);
    return mFullLic;
}

string getLicense(JNIEnv *env, const string& fullLicense) {
    if (fullLicense.empty()) {
        return "";
    }

    string* split = strSplit(env, fullLicense, nDiv);
    int len = splitLength;
    len--;

    string lic = "";
    for (int i=0; i<len; i++) {
        if (lic!="") {
            lic += "|";
        }
        lic += split[i];
    }

    return lic;
}

string getSignature(JNIEnv *env, const string& fullLicense) {
    if (fullLicense.empty()) {
        return "";
    }

    string* split = strSplit(env, fullLicense, nDiv);
    int len = splitLength;
    len--;

    return split[len];
}

string getLicenseItem(JNIEnv *env, const string& fullLicense, int index, bool nullable) {
    if (fullLicense.empty()) {
        if (nullable) {
            return "null";
        } else {
            return "";
        }
    }

    string fL = strReplace(env, fullLicense, nDiv+nDiv, nDiv + " " + nDiv);
    string* split = strSplit(env, fullLicense, nDiv);
    int len = splitLength;

    if (len<=index) {
        if (nullable) {
            return "null";
        } else {
            return "";
        }
    }

    string res = split[index];
    return res;
}

bool iUTDate(JNIEnv *env, const string& fullLicense) {
    string expDate = getLicenseItem(env, fullLicense, 3, true);
    if (expDate.empty()) {
        return true;
    }

    if (expDate=="null") {
        return false;
    }

    jclass Date = env->FindClass("java/util/Date");
    jmethodID after = env->GetMethodID(Date, "after", "(Ljava/util/Date;)Z");
    string today = getToday(env);

    jclass SDF = env->FindClass("java/text/SimpleDateFormat");
    jmethodID sdfInit = env->GetMethodID(SDF, "<init>", "(Ljava/lang/String;)V");
    jmethodID parse = env->GetMethodID(SDF, "parse", "(Ljava/lang/String;)Ljava/util/Date;");
    jobject sdf = env->NewObject(SDF, sdfInit, env->NewStringUTF("yyyyMMdd"));

    if (env->ExceptionCheck())
        env->ExceptionClear();

    bool res = !env->CallBooleanMethod(env->CallObjectMethod(sdf, parse, env->NewStringUTF(today.c_str())), after, env->CallObjectMethod(sdf, parse, env->NewStringUTF(expDate.c_str())));

    if (env->ExceptionCheck())
        env->ExceptionClear();

    env->DeleteLocalRef(Date);
    env->DeleteLocalRef(SDF);
    env->DeleteLocalRef(sdf);

    return res;
}

jboolean isLicenseOk(JNIEnv *env, jobject context, const string& fullLicense, bool justSignature) {
    //para que esté optimizado
    if (validLic) {
        return true;
    }

    CkGlobal cglob;
    bool success = cglob.UnlockBundle("ANEMNA.CB4032026_ciuicL5Fndll");
    if (success != 1) {
        cglob.dispose();
        return false;
    }
    cglob.dispose();

    //Checa que tenga una licencia y que ésta sea de Android
    string platform = getLicenseItem(env, fullLicense, 0, false);
    if (fullLicense.empty() || str2LowerCase(env, platform)!="android") {
        return false;
    }

    //Valida la firma de la licencia
    string license = getLicense(env, fullLicense);
    string signature = getSignature(env, fullLicense);

    unsigned char pk[] = {
                    0x62, 0x7f, 0x4b, 0x4a, 0xf2, 0x6e, 0xb, 0x84,
                    0x24, 0x54, 0x22, 0x9d, 0x16, 0xaf, 0x53, 0xaf,
                    0x4b, 0xc9, 0xa6, 0xa6, 0x39, 0x3e, 0x7a, 0x3a,
                    0xd2, 0x3a, 0xd3, 0x54, 0x7, 0xee, 0xb, 0xc9,
                    0xd9, 0x7c, 0xff, 0x5c, 0x55, 0x36, 0xf0, 0xaf,
                    0x2b, 0xa9, 0xc4, 0xbe, 0xfd, 0x1c, 0x78, 0x77,
                    0x92, 0x4f, 0x8c, 0x59, 0x5, 0x83, 0xc0, 0xba,
                    0xf9, 0x99, 0xc2, 0xd4, 0xae, 0xcc, 0xa8, 0x36,
                    0x40, 0x22, 0xbb, 0xfc, 0xb5, 0x73, 0xff, 0x10,
                    0xe9, 0x2a, 0xb2, 0xa3, 0x7d, 0xd9, 0xd9, 0x45,
                    0xb2, 0x51, 0x3a, 0x1a, 0xa6, 0x63, 0xe1, 0xce,
                    0xb9, 0x37, 0x54, 0x22, 0x6e, 0x4d, 0x88, 0xa3,
                    0xc1, 0xa2, 0xfd, 0xdc, 0x15, 0xb6, 0x8f, 0x8a,
                    0x89, 0x49, 0x84, 0x5e, 0x7d, 0x1b, 0x99, 0x98,
                    0x50, 0xd0, 0x8c, 0x59, 0xc6, 0xa6, 0xcf, 0x0,
                    0xf9, 0x19, 0x54, 0x53, 0xcd, 0x2d, 0xf7, 0xc8,
                    0x62, 0x82, 0x9d, 0x9e, 0x68, 0xa9, 0xf, 0xb2,
                    0xa9, 0xc8, 0x85, 0xe4, 0x1e, 0x1b, 0x78, 0x73,
                    0x10, 0xd2, 0xac, 0xac, 0x86, 0x65, 0x80, 0xff,
                    0x1a, 0xf9, 0x34, 0x33, 0xec, 0x48, 0x97, 0xe7,
                    0x2, 0x5c, 0x6b, 0x56, 0xd5, 0x76, 0x9f, 0xef,
                    0x89, 0xa8, 0x52, 0xb2, 0x3d, 0x59, 0x78, 0xa6,
                    0xb1, 0xf, 0x6c, 0x4b, 0xe6, 0xc6, 0x20, 0x3f,
                    0xf8, 0xf9, 0xe2, 0xd3, 0x6f, 0x49, 0x69, 0x56,
                    0x61, 0x80, 0x8b, 0x4a, 0xb5, 0x74, 0x5f, 0xb0,
                    0x49, 0x29, 0x23, 0x92, 0xfc, 0xbb, 0x99, 0xb7,
                    0xb0, 0x70, 0xec, 0xf9, 0x65, 0xa6, 0x60, 0x1a,
                    0x3a, 0x38, 0x43, 0x34, 0x8d, 0xed, 0x28, 0xb5,
                    0x63, 0xc1, 0x7d, 0x7c, 0x15, 0xb5, 0x71, 0x90,
                    0x1d, 0xa4, 0x82, 0x3d, 0x3c, 0x5c, 0x70, 0xa5,
                    0x71, 0x70, 0x3b, 0x7, 0x43, 0x3, 0x2f, 0x13,
                    0x13, 0xd7, 0x51, 0x8c, 0x6f, 0x9, 0x17, 0xc1,
                    0x80, 0xd9, 0xd6, 0xd5, 0xb4, 0xcc, 0xde, 0xa9,
                    0x69, 0x8, 0x62, 0x32, 0xd6, 0xdb, 0x47, 0x2b,
                    0x2b, 0xcf, 0x69, 0xa4, 0x86, 0x21, 0xef, 0xfc,
                    0x78, 0x38, 0x71, 0xf0, 0x4d, 0xaa, 0xa9, 0xa4,
                    0xa3, 0xbe, 0xbd, 0xb8, 0xb7, 0xb2, 0xb1, 0x8c,
                    0x8b, 0x86, 0x85, 0x80, 0x7f, 0x7a, 0x79, 0xb4,
                    0xb3, 0xce, 0xcd, 0xc8, 0xc7, 0xc2, 0xc1, 0x9c,
                    0x9b, 0x96, 0x95, 0x90, 0x8f, 0x8a, 0x89, 0x84,
                    0x83, 0x9e, 0x9d, 0x98, 0x97, 0x92, 0x91, 0x87,
                    0x6b, 0x66, 0x65, 0x60, 0x7d, 0xdc, 0xe7, 0xe6,
                    0xd1, 0xd0, 0xdb, 0xda, 0xe5, 0xe4, 0xaf, 0x9b,
                    0x7b, 0x76, 0x75, 0x70, 0x8d, 0x65, 0x22, 0x7f,
                    0xbf, 0x79, 0x7d, 0x78, 0x77, 0x72, 0x71, 0x4c,
                    0x4b, 0x46, 0x45, 0x40, 0x3f, 0x3a, 0x39, 0x74,
                    0x73, 0x8e, 0x8d, 0x88, 0x87, 0x82, 0x81, 0x5c,
                    0x5b, 0x56, 0x55, 0x50, 0x4f, 0x4a, 0x49, 0x44,
                    0x43, 0x5e, 0x5d, 0x58, 0x57, 0x52, 0x51, 0x2c,
                    0x2b, 0x26, 0x25, 0x20, 0x1e, 0x1a, 0x19, 0x54,
                    0x53, 0x6e, 0x9b, 0x9a, 0xa5, 0xa4, 0x6f, 0x6e,
                    0x79, 0x78, 0x62, 0x61, 0xcd, 0x2a, 0x29, 0x24,
                    0x23, 0x5c, 0x8b, 0xca, 0xb5, 0xa9, 0xce, 0x8c,
                    0x8c, 0x1e, 0xa5, 0x99, 0xfe, 0x32, 0xf8, 0x91,
                    0x8c, 0x4a, 0x9b, 0x47, 0xfe, 0x7b, 0xc2, 0x8e,
                    0x3a, 0x71, 0x2, 0x62, 0x6f, 0x98, 0x63, 0xff,
                    0xe2, 0x60, 0x59, 0x78, 0x54, 0xb4, 0xbe, 0x1d,
                    0x64, 0x5, 0xe0, 0x31, 0x1b, 0x37, 0x14, 0xb0,
                    0x8e, 0x2d, 0x29, 0x63, 0x87, 0x7f, 0x9e, 0x98,
                    0xf8, 0x84, 0x72, 0xe8, 0x9, 0x27, 0xe8, 0x82,
                    0x1e, 0x7a, 0x7a, 0xb3, 0x15, 0x94, 0x9e, 0xb,
                    0x49, 0x5f, 0xa1, 0x5b, 0x5c, 0x36, 0x11, 0x2b,
                    0xcb, 0x47, 0xc6, 0x0, 0x27, 0x44, 0x1a, 0xdb,
                    0xf2, 0xae, 0x4f, 0xb3, 0x4b, 0x4c, 0x82, 0x60,
                    0x23, 0x5a, 0x57, 0xd3, 0x33, 0xca, 0xff, 0x29,
                    0x23, 0xc2, 0x1c, 0x19, 0x5c, 0x1b, 0x12, 0xef,
                    0xae, 0x52, 0x46, 0x44, 0x4, 0xde, 0xdd, 0x97,
                    0x77, 0xa3, 0xd, 0xca, 0xb, 0x65, 0xc5, 0x44,
                    0x5b, 0xf7, 0xf6, 0x9b, 0x41, 0xe0, 0x4a, 0x5d,
                    0xa3, 0xa, 0x1, 0x7d, 0x3d, 0x3c, 0x94, 0x46,
                    0xcb, 0x51, 0xa8, 0x8, 0x63, 0xd9, 0xbb, 0xfa,
                    0xda, 0x75, 0x12, 0x8f, 0x86, 0x6d, 0x64, 0x75,
                    0x23, 0x22, 0x17, 0x50, 0xd2, 0xef, 0x48, 0x3,
                    0xa5, 0xc1, 0xe5, 0x19, 0xd7, 0x99, 0xf6, 0xed,
                    0xca, 0x89, 0xc8, 0x3, 0x86, 0xbd, 0x58, 0x5f,
                    0xf8, 0xf6, 0xf5, 0xe2, 0x47, 0xe, 0xa8, 0xd6,
                    0xc3, 0xbd, 0xfa, 0xd5, 0x4f, 0x31, 0x2a, 0x48,
                    0x63, 0xb4, 0x2, 0xe0, 0x7b, 0x7d, 0x98, 0x30,
                    0xec, 0xb0, 0xfb, 0x20, 0x5, 0x78, 0xb8, 0x74,
                    0x9b, 0xd7, 0x4e, 0xab, 0x8f, 0x2d, 0x85, 0x7a,
                    0x83, 0x7c, 0xd6, 0x54, 0x2f, 0x2e, 0x4b, 0x67,
                    0x67, 0x7d, 0x52, 0x80, 0x1f, 0x1a, 0x19, 0x54,
                    0x53, 0x6e, 0x6d, 0x68, 0x67, 0x62, 0x61, 0x3c,
                    0x3b, 0x36, 0x35, 0x30, 0x2f, 0x2a, 0x29, 0x24,
                    0x23, 0x3e, 0x3d, 0x38, 0x37, 0x32, 0x31, 0xc,
                    0xb, 0x6, 0x5, 0x1e, 0xe8, 0x93, 0xd4, 0x8f,
                    0x50, 0x52, 0x29, 0x5f, 0xc7, 0xef, 0x1c, 0x6e,
                    0x13, 0xf2, 0x8d, 0x89, 0x48, 0x5, 0x40, 0x88,
                    0x7b, 0x9a, 0x1c, 0x6a, 0x90, 0x2d, 0x88, 0xa9,
                    0xa7, 0x42, 0x9f, 0x1b, 0x5c, 0x7c, 0x67, 0x54,
                    0xf, 0x67, 0x5b, 0x5a, 0x83, 0xde, 0xfd, 0x16,
                    0x37, 0x58, 0xad, 0x4c, 0xca, 0xe9, 0x29, 0x82,
                    0x5b, 0xd6, 0x36, 0x7c, 0xcf, 0x32, 0xc9, 0xa8,
                    0x8b, 0x2, 0x42, 0x20, 0xde, 0x19, 0xf4, 0x2b,
                    0x4c, 0x6e, 0x6d, 0xd9, 0x87, 0xfe, 0xa0, 0x3a,
                    0x33, 0x8e, 0x94, 0x34, 0x4d, 0x38, 0x96, 0x1b,
                    0xe2, 0x19, 0x95, 0xf2, 0xd4, 0xf, 0xf0, 0xdd,
                    0x2c, 0x2a, 0x26, 0x20, 0xd6, 0x19, 0xb4, 0x58,
                    0xa, 0x89, 0xa9, 0x3a, 0x4, 0x9e, 0x1e, 0xd6,
                    0x9a, 0xf3, 0x55, 0x73, 0xac, 0xc6, 0xe1, 0xb5,
                    0x9e, 0x22, 0x9c, 0x55, 0x92, 0x8a, 0x9, 0x68,
                    0xa7, 0x65, 0x1c, 0x1e, 0x5e, 0xf5, 0x91, 0xd3,
                    0x34, 0xeb, 0x9, 0x2c, 0x70, 0x26, 0x0, 0x1c,
                    0x14, 0xd2, 0x6c, 0x4c, 0xe7, 0xf8, 0xbf, 0x21,
                    0x80, 0x4b, 0x9d, 0x38, 0x33, 0xc0, 0x8b, 0x6f,
                    0x67, 0xe1, 0x5d, 0xfc, 0xbc, 0xb5, 0x91, 0xd,
                    0xee, 0x5, 0xc6, 0x47, 0xe3, 0x60, 0x3f, 0x7c,
                    0x38, 0xf8, 0xe3, 0x8f, 0x26, 0xd8, 0x2, 0xbd,
                    0xed, 0xfb, 0x36, 0x8e, 0x55, 0x7b
            };

    for (unsigned int m = 0; m < sizeof(pk); ++m) {
        unsigned char c = pk[m];
        c += m;
        c = (c >> 0x5) | (c << 0x3);
        c -= 0xa5;
        c = ~c;
        c = -c;
        c = (c >> 0x3) | (c << 0x5);
        c ^= 0x68;
        c = -c;
        c ^= 0x96;
        c += m;
        c ^= 0xe1;
        c -= 0x93;
        c = -c;
        c -= m;
        c = (c >> 0x5) | (c << 0x3);
        pk[m] = c;
    }

    CkCrypt2 crypt;
    crypt.put_Charset("utf-8");
    crypt.put_HashAlgorithm("sha256");
    crypt.put_EncodingMode("base64");
    string hash = crypt.hashStringENC(license.c_str());

    CkPublicKey pubKey;
    if (!pubKey.LoadXml((const char*) pk)) {
        return false;
    }

    CkEcc ecc2;
    int result = ecc2.VerifyHashENC(hash.c_str(), signature.c_str(), "base64", pubKey);
    if (result != 1) {
        return false;
    }

    if (justSignature) {
        return true;
    }

    //Valida el bundleID de la aplicación y de la licencia
    string appB = getBundleID(env, context);
    validLic = getLicenseItem(env, fullLicense, 1, false)==appB;
    if (validLic) {
        validLic = iUTDate(env, fullLicense);
        if (env->ExceptionCheck())
            env->ExceptionClear();
    }
    return validLic;
}

int getLicVals(JNIEnv *env, jobject context) {
    string fullLicense = getFullLicense(env, context);

    if (fullLicense.empty() || !isLicenseOk(env, context, fullLicense, true)) {
        return -1;
    }

    string lN = getLicenseItem(env, fullLicense, 4, false);
    if (lN.empty()) {
        return -1;
    }

    return str2Int(env, lN);
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_personalcode_Azf_isUpToDate(JNIEnv *env, jobject thiz, jobject activity) {
    string fullLicense = getFullLicense(env, activity);

    if (fullLicense.empty() || !isLicenseOk(env, activity, fullLicense, true)) {
        return false;
    }
    return iUTDate(env, fullLicense);
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_personalcode_Azf_getValidity(JNIEnv *env, jobject thiz, jobject activity) {
    string fullLicense = getFullLicense(env, activity);

    if (fullLicense.empty() || !isLicenseOk(env, activity, fullLicense, true)) {
        return nullptr;
    }
    string expDate = getLicenseItem(env, fullLicense, 5, false);
    return env->NewStringUTF(expDate.c_str());
}
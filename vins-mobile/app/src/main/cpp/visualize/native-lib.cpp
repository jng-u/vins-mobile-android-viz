#include <jni.h>
#include "utils.hpp"
#include "application.hpp"

static application* m_app = NULL;

extern "C" {
    JNIEXPORT void JNICALL Java_com_thkoeln_jmoeller_vins_1mobile_1androidport_VisualizeLib_init(JNIEnv* env, jobject obj);
    JNIEXPORT void JNICALL Java_com_thkoeln_jmoeller_vins_1mobile_1androidport_VisualizeLib_resize(JNIEnv* env, jobject obj, jint width, jint height);
    JNIEXPORT void JNICALL Java_com_thkoeln_jmoeller_vins_1mobile_1androidport_VisualizeLib_render(JNIEnv* env, jobject obj);
    JNIEXPORT void JNICALL Java_com_thkoeln_jmoeller_vins_1mobile_1androidport_VisualizeLib_cameraMove(JNIEnv* env, jobject obj, jfloat dx, jfloat dy);
};

JNIEXPORT void JNICALL Java_com_thkoeln_jmoeller_vins_1mobile_1androidport_VisualizeLib_init(JNIEnv* env, jobject obj) {
    if (m_app) {
        delete m_app;
        m_app = NULL;
    }

    glesLog("Version : %s", (const char*)glGetString(GL_VERSION));
    glesLog("Vendor : %s", (const char*)glGetString(GL_VENDOR));
    glesLog("Renderer : %s", (const char*)glGetString(GL_RENDERER));
//    glesLog("Extensions : %s", (const char*)glGetString(GL_EXTENSIONS));

    const char* versionStr = (const char*)glGetString(GL_VERSION);
    if (strstr(versionStr, "OpenGL ES 3.")) {
        m_app = new application();
        m_app->init();
    } else {
        glesLog("Unsupported OpenGL ES 3 version");
    }
}

JNIEXPORT void JNICALL Java_com_thkoeln_jmoeller_vins_1mobile_1androidport_VisualizeLib_resize(JNIEnv* env, jobject obj, jint width, jint height) {
    if (m_app) {
        glesLog("Call JNI resize function (width : %d, height : %d)", width, height);
        m_app->resize(width, height);
    }
}

JNIEXPORT void JNICALL Java_com_thkoeln_jmoeller_vins_1mobile_1androidport_VisualizeLib_render(JNIEnv* env, jobject obj) {
    if (m_app) {
//        glesLog("Call JNI render function");
        m_app->draw();
    }
}

JNIEXPORT void JNICALL Java_com_thkoeln_jmoeller_vins_1mobile_1androidport_VisualizeLib_cameraMove(JNIEnv* env, jobject obj, jfloat dx, jfloat dy) {
    if (m_app) {
        m_app->cameraRotation(dx, dy);
    }
}

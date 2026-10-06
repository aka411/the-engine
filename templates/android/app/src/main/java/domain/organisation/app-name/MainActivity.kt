package @ANDROID_PACKAGE_NAME@

import android.os.Bundle
import android.os.PersistableBundle
import androidx.core.view.WindowCompat
import androidx.core.view.WindowInsetsCompat
import androidx.core.view.WindowInsetsControllerCompat
import com.google.androidgamesdk.GameActivity


class MainActivity : GameActivity() {
    companion object {
        init {
            System.loadLibrary("@ANDROID_APP_SHARED_LIBRARY@")
        }
    }

    override fun onCreate(savedInstanceState: Bundle?, persistentState: PersistableBundle?){
        super.onCreate(savedInstanceState, persistentState)
        WindowCompat.enableEdgeToEdge(window)

    }

    override fun onWindowFocusChanged(hasFocus: Boolean) {
        super.onWindowFocusChanged(hasFocus)
        if (hasFocus) {
            hideSystemUi()
        }
    }

    private fun hideSystemUi() {

            WindowCompat.getInsetsController(window,window.decorView).apply {

            hide(WindowInsetsCompat.Type.systemBars())
            systemBarsBehavior = WindowInsetsControllerCompat.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE
        }
    }
}
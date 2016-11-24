package com.helloworld;

import com.facebook.react.ReactActivity;
import android.view.View;
import android.annotation.SuppressLint;
import android.os.Bundle;
import android.os.Build;

public class MainActivity extends ReactActivity {
//    private int currentApiVersion;

    @Override
    @SuppressLint("NewApi")
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

//        currentApiVersion = android.os.Build.VERSION.SDK_INT;
//
//        final int flags = View.SYSTEM_UI_FLAG_LAYOUT_STABLE
//                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
//                | View.SYSTEM_UI_FLAG_LAYOUT_STABLE
//                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
//                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
//                | View.SYSTEM_UI_FLAG_FULLSCREEN
//                | View.SYSTEM_UI_FLAG_IMMERSIVE;
//
//        // This work only for android 4.4+
//        if(currentApiVersion >= Build.VERSION_CODES.KITKAT)
//        {
//
//            getWindow().getDecorView().setSystemUiVisibility(flags);
//
//            // Code below is to handle presses of Volume up or Volume down.
//            // Without this, after pressing volume buttons, the navigation bar will
//            // show up and won't hide
//            final View decorView = getWindow().getDecorView();
//            decorView
//                    .setOnSystemUiVisibilityChangeListener(new View.OnSystemUiVisibilityChangeListener()
//                    {
//
//                        @Override
//                        public void onSystemUiVisibilityChange(int visibility)
//                        {
//                            if((visibility & View.SYSTEM_UI_FLAG_FULLSCREEN) == 0)
//                            {
//                                decorView.setSystemUiVisibility(flags);
//                            }
//                        }
//                    });
//        }

    }


//    @SuppressLint("NewApi")
//    @Override
//    public void onWindowFocusChanged(boolean hasFocus)
//    {
//        super.onWindowFocusChanged(hasFocus);
//        if(currentApiVersion >= Build.VERSION_CODES.KITKAT && hasFocus)
//        {
//            getWindow().getDecorView().setSystemUiVisibility(
//                    View.SYSTEM_UI_FLAG_LAYOUT_STABLE
//                            | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
//                            | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
//                            | View.SYSTEM_UI_FLAG_LAYOUT_STABLE
//                            | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
//                            | View.SYSTEM_UI_FLAG_FULLSCREEN
//                            | View.SYSTEM_UI_FLAG_IMMERSIVE);
//        }
//    }
    /**
     * Returns the name of the main component registered from JavaScript.
     * This is used to schedule rendering of the component.
     */
    @Override
    protected String getMainComponentName() {
        return "helloworld";
    }

}

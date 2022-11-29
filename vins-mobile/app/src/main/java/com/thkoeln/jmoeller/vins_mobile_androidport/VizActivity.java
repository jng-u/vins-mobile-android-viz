package com.thkoeln.jmoeller.vins_mobile_androidport;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import java.io.File;

public class VizActivity extends Fragment {

    VisualizeView mView;

    @Override
    public View onCreateView(@Nullable LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        mView = new VisualizeView(this.getActivity());
        return mView;
    }

    //
//    @Override protected void onCreate(Bundle icicle) {
//        super.onCreate(icicle);
//        mView = new VisualizeView(getApplication());
//        setContentView(mView);
//    }
//
//    @Override protected void onPause() {
//        super.onPause();
//        mView.onPause();
//    }
//
//    @Override protected void onResume() {
//        super.onResume();
//        mView.onResume();
//    }
}
/**
 * \file  CommandScreenCameraRobot1Fragment.java
 * \version  0.1
 * \author Matéo R
 * \date May 18, 2023
 * \brief The CommandScreenCameraFragment class corresponds to the fragment
 * used to display the camera stream received from the first robot.
 *
 * \section License
 *
 * The MIT License
 *
 * Copyright (c) 2023, Prose A2 2023
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * \copyright Prose A2 2023
 *
 */

/* ----------------------  PACKAGE  ---------------------------------------- */

package project.src.view.fragments;

/* ----------------------  INCLUDES  ---------------------------------------- */

import android.graphics.SurfaceTexture;
import android.media.MediaCodec;
import android.media.MediaFormat;
import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;

import android.view.LayoutInflater;
import android.view.Surface;
import android.view.TextureView;
import android.view.View;
import android.view.ViewGroup;

import java.io.IOException;
import java.nio.ByteBuffer;

import project.src.R;

public class CommandScreenCameraRobot1Fragment extends Fragment implements TextureView.SurfaceTextureListener{

    /* ----------------------  PRIVATE OBJECTS  ------------------------------ */

    /**
     * @brief Texture view for the video.
     */
    private TextureView textureView;

    /**
     * @brief Decoder for the video.
     */
    private static MediaCodec decoder1;

    /**
     * @brief View of the fragment.
     */
    private View view;

    /* ----------------------  PRIVATE VARIABLES  ---------------------------- */

    /* ----------------------  CONSTRUCTOR(S)  -------------------------------- */

    /**
     * @brief Constructor of the class.
     */
    public CommandScreenCameraRobot1Fragment() {
        // Required empty public constructor
    }

    /* ----------------------  ON CREATE VIEW ---------------------------- */

    /**
     * @brief Creation on the view of the fragment.
     * @param inflater The LayoutInflater object that can be used to inflate
     * any views in the fragment,
     * @param container If non-null, this is the parent view that the fragment's
     * UI should be attached to.  The fragment should not add the view itself,
     * but this can be used to generate the LayoutParams of the view.
     * @param savedInstanceState If non-null, this fragment is being re-constructed
     * from a previous saved state as given here.
     *
     * @return view of the fragment.
     */
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        view = inflater.inflate(R.layout.fragment_command_screen_camera_robot1, container, false);
        this.textureView = view.findViewById(R.id.textureView);
        this.textureView.setSurfaceTextureListener(this);

        // Inflate the layout for this fragment
        return view;
    }

    /* ----------------------  PUBLIC FUNCTIONS  -------------------------------- */

    /**
     * @brief Setup the decoder when the surface texture is available.
     * @param surfaceTexture Surface to update to show the image on the screen
     */
    @Override
    public void onSurfaceTextureAvailable(@NonNull SurfaceTexture surfaceTexture, int i, int i1) {
        MediaFormat format = MediaFormat.createVideoFormat(MediaFormat.MIMETYPE_VIDEO_AVC, 320, 240);
        format.setInteger(MediaFormat.KEY_FRAME_RATE, 10);

        try {
            decoder1 = MediaCodec.createDecoderByType(MediaFormat.MIMETYPE_VIDEO_AVC);
            decoder1.configure(format, new Surface(surfaceTexture), null, 0);
            decoder1.start();
        } catch (IOException e) {

        }
    }

    /**
     * @brief Listen if the size of the surface texture have changed.
     * @param surfaceTexture
     */
    @Override
    public void onSurfaceTextureSizeChanged(@NonNull SurfaceTexture surfaceTexture, int i, int i1) {

    }

    /**
     * @brief Listen if the surface texture is destroyed.
     * @param surfaceTexture
     * @return false.
     */
    @Override
    public boolean onSurfaceTextureDestroyed(@NonNull SurfaceTexture surfaceTexture) {
        return false;
    }

    /**
     * @brief Listen if the surface texture is updated.
     * @param surfaceTexture
     */
    @Override
    public void onSurfaceTextureUpdated(@NonNull SurfaceTexture surfaceTexture) {

    }

    /**
     * @brief Decode the data received by the UDPReceiver.
     * @param frame Video data for the decoder.
     * @param length Length of the video data for the decoder.
     */
    public static void decodingBytes(byte[] frame, int length){
        int inputIndex = decoder1.dequeueInputBuffer(-1);
        if(inputIndex>=0){
            ByteBuffer buffer = decoder1.getInputBuffer(inputIndex);
            buffer.clear();
            buffer.put(frame);
            decoder1.queueInputBuffer(inputIndex, 0, length, 0, 0);
        }

        MediaCodec.BufferInfo info = new MediaCodec.BufferInfo();
        int outputIndex;
        while ((outputIndex = decoder1.dequeueOutputBuffer(info, 0))>=0){
            decoder1.releaseOutputBuffer(outputIndex, true);
        }
    }

    /**
     * @brief Release the decoder.
     */
    public void releaseDecode(){
        decoder1.stop();
        decoder1.release();
    }

    /* ----------------------  PROTECTED FUNCTIONS  ------------------------------- */

    /* ----------------------  PRIVATE FUNCTIONS  ------------------------------- */

}
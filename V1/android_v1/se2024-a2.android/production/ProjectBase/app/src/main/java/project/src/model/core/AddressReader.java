/**
 * \file  AddressReader.java
 * \version  0.1
 * \author Matéo R
 * \date Jun 7, 2023
 * \brief Class to obtain the IP address of the current tablet or phone.
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

package project.src.model.core;

/* ----------------------  INCLUDES  ---------------------------------------- */

import android.content.Context;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;

import java.net.InetAddress;
import java.net.UnknownHostException;

public class AddressReader {

    /* ----------------------  PRIVATE OBJECTS  ---------------------------------------- */

    /**
     * @brief Context of the class.
     */
    private static Context context = null;

    /* ----------------------  PRIVATE VARIABLES  ---------------------------------------- */

    /* ----------------------  CONSTRUCTOR  ---------------------------------------- */

    /**
     * @brief Constructor of the class.
     */
    public AddressReader(){
    }

    /* ----------------------  PUBLIC FUNCTIONS  ---------------------------------------- */

    /**
     * @brief Permit to retrieve the IP address of the tablet.
     * @return ipString.
     */
    public String getIPAddress(){
        WifiManager wifiManager = null;
        int ipAddress;
        if (context != null) {
            wifiManager = (WifiManager) context.getSystemService(Context.WIFI_SERVICE);
            WifiInfo wifiInfo = wifiManager.getConnectionInfo();
            ipAddress = wifiInfo.getIpAddress();
        } else ipAddress = 0;

        String ipString = null;
        try {
            ipString = InetAddress.getByAddress(
                    new byte[]{(byte) (ipAddress & 0xFF), (byte) (ipAddress >> 8 & 0xFF),
                            (byte) (ipAddress >> 16 & 0xFF), (byte) (ipAddress >> 24 & 0xFF)}).getHostAddress();
        } catch (UnknownHostException e) {
            e.printStackTrace();
        }
        return ipString;
    }

    /**
     * @brief Set the context into the static context of the object.
     * @param cxt Context to set.
     */
    public static void setContext(Context cxt){
        if (cxt != null)  context = cxt;
    }

    /* ----------------------  PROTECTED FUNCTIONS  ---------------------------------------- */

    /* ----------------------  PRIVATE FUNCTIONS  ---------------------------------------- */
}

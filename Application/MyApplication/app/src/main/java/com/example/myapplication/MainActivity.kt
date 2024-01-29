package com.example.myapplication

import HttpRequestTask
import HttpSendTask
import android.annotation.SuppressLint
import android.os.Bundle
import android.widget.Button
import android.widget.ImageView
import android.widget.LinearLayout
import android.widget.ProgressBar
import android.widget.Switch
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.lifecycleScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch
import kotlinx.coroutines.runBlocking
import kotlinx.coroutines.withContext
import java.text.DecimalFormat
import org.json.JSONObject



/*class MainActivity : AppCompatActivity() {
    @OptIn(DelicateCoroutinesApi::class)
    @SuppressLint("MissingInflatedId", "SetTextI18n")
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        findViewById<TextView>(R.id.textview_hello).text = "COucouuuuuu !!!";
        findViewById<Button>(R.id.button_1).setOnClickListener(View.OnClickListener {
            findViewById<TextView>(
                R.id.textview_hello
            ).text = "We clicked on the button !!"
        })


        Log.d("test", "testttt")
        //findViewById<TextView>(R.id.textview_hello).text = valPrint
        GlobalScope.launch {

            loopPrintData()

        }

    }

    private suspend fun loopPrintData() {
        while(true) {
            val url = "http://138.197.181.206/api/allData"
            val httpRequestTask = HttpRequestTask()
            httpRequestTask.execute(url)
            //findViewById<TextView>(R.id.textview_hello).text = GeicarData.carData["us_front_left"].toString()
            delay(200)
        }

    }




}*/

class MainActivity : AppCompatActivity() {

    val decimalFormat = DecimalFormat("#.#")
    var boolEmergency: Boolean = false
    var boolRecord: Boolean = false
    var boolReplay: Boolean = false
    var boolReverse: Boolean = false
    var boolForward: Boolean = false
    var boolSensors: Boolean = false


    @SuppressLint("MissingInflatedId", "SetTextI18n")
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        var httpst: HttpSendTask = HttpSendTask()

        findViewById<Button>(R.id.button_emergency).setOnClickListener {
            if (!boolEmergency) {
                val jsonObject = JSONObject()
                val url = "http://138.197.181.206/api/data/button_emergency"
                jsonObject.put("button_emergency", true)
                val thread = Thread {
                    runBlocking {
                        httpst.sendData(Pair(url, jsonObject))
                    }
                }
                thread.start()
                findViewById<Button>(R.id.button_emergency).text = "Restart"
                boolEmergency = true
            } else {
                val jsonObject = JSONObject()
                val url = "http://138.197.181.206/api/data/button_emergency"
                jsonObject.put("button_emergency", false)
                val thread = Thread {
                    runBlocking {
                        httpst.sendData(Pair(url, jsonObject))
                    }
                }
                thread.start()
                findViewById<Button>(R.id.button_emergency).text = "Emergency"
                boolEmergency = false
            }

        }



        findViewById<Button>(R.id.button_record).setOnClickListener {
            if (!boolRecord ) {
                val jsonObject = JSONObject()
                val url = "http://138.197.181.206/api/data/button_record"
                jsonObject.put("button_record", true)
                val thread = Thread {
                    runBlocking {
                        httpst.sendData(Pair(url, jsonObject))
                    }
                }
                thread.start()
                findViewById<Button>(R.id.button_record).text = "Stop"
                boolRecord = true
            } else {
                val jsonObject = JSONObject()
                val url = "http://138.197.181.206/api/data/button_record"
                jsonObject.put("button_record", false)
                val thread = Thread {
                    runBlocking {
                        httpst.sendData(Pair(url, jsonObject))
                    }
                }
                thread.start()
                findViewById<Button>(R.id.button_record).text = "Record"
                boolRecord = false
            }

        }

        findViewById<Button>(R.id.button_replay).setOnClickListener {
            if (!boolReplay ) {
                val jsonObject = JSONObject()
                val url = "http://138.197.181.206/api/data/button_replay"
                jsonObject.put("button_replay", true)
                val thread = Thread {
                    runBlocking {
                        httpst.sendData(Pair(url, jsonObject))
                    }
                }
                thread.start()
                findViewById<Button>(R.id.button_replay).text = "Stop"
                boolReplay = true
            } else {
                val jsonObject = JSONObject()
                val url = "http://138.197.181.206/api/data/button_replay"
                jsonObject.put("button_replay", false)
                val thread = Thread {
                    runBlocking {
                        httpst.sendData(Pair(url, jsonObject))
                    }
                }
                thread.start()
                findViewById<Button>(R.id.button_replay).text = "Replay"
                boolReplay = false
            }

        }

        findViewById<Button>(R.id.button_reverse).setOnClickListener {
            if (!boolReverse ) {
                val jsonObject = JSONObject()
                val url = "http://138.197.181.206/api/data/button_reverse"
                jsonObject.put("button_reverse", true)
                val thread = Thread {
                    runBlocking {
                        httpst.sendData(Pair(url, jsonObject))
                    }
                }
                thread.start()
                findViewById<Button>(R.id.button_reverse).text = "Stop"
                boolReverse = true
            } else {
                val jsonObject = JSONObject()
                val url = "http://138.197.181.206/api/data/button_reverse"
                jsonObject.put("button_reverse", false)
                val thread = Thread {
                    runBlocking {
                        httpst.sendData(Pair(url, jsonObject))
                    }
                }
                thread.start()
                findViewById<Button>(R.id.button_reverse).text = "Reverse"
                boolReverse = false
            }

        }

        findViewById<Button>(R.id.button_forward).setOnClickListener {
            if (!boolForward ) {
                val jsonObject = JSONObject()
                val url = "http://138.197.181.206/api/data/button_forward"
                jsonObject.put("button_forward", true)
                val thread = Thread {
                    runBlocking {
                        httpst.sendData(Pair(url, jsonObject))
                    }
                }
                thread.start()
                findViewById<Button>(R.id.button_forward).text = "Stop"
                boolForward = true
            } else {
                val jsonObject = JSONObject()
                val url = "http://138.197.181.206/api/data/button_forward"
                jsonObject.put("button_forward", false)
                val thread = Thread {
                    runBlocking {
                        httpst.sendData(Pair(url, jsonObject))
                    }
                }
                thread.start()
                findViewById<Button>(R.id.button_forward).text = "Forward"
                boolForward = false
            }

        }

        findViewById<Button>(R.id.button_sensors).setOnClickListener {
            if (!boolSensors ) {
                val jsonObject = JSONObject()
                val url = "http://138.197.181.206/api/data/button_sensors"
                jsonObject.put("button_sensors", true)
                val thread = Thread {
                    runBlocking {
                        httpst.sendData(Pair(url, jsonObject))
                    }
                }
                thread.start()
                findViewById<Button>(R.id.button_sensors).text = "Sensors OFF"
                boolSensors = true
            } else {
                val jsonObject = JSONObject()
                val url = "http://138.197.181.206/api/data/button_sensors"
                jsonObject.put("button_sensors", false)
                val thread = Thread {
                    runBlocking {
                        httpst.sendData(Pair(url, jsonObject))
                    }
                }
                thread.start()
                findViewById<Button>(R.id.button_sensors).text = "Sensors ON"
                boolSensors = false
            }

        }


        lifecycleScope.launch {
            loopPrintData()
        }


    }

    @SuppressLint("SetTextI18n", "CutPasteId")
    private suspend fun loopPrintData() {
        while (true) {
            val url = "http://138.197.181.206/api/allData"

            // Utilisez une fonction de suspension pour les appels réseau
            val data = fetchData(url)

            // Mettez à jour l'interface utilisateur sur le thread principal
            withContext(Dispatchers.Main) {
                if (Integer.parseInt(GeicarData.carData["us_front_left"].toString()) > 70) {
                    findViewById<ImageView>(R.id.image_front_left).setImageResource(R.drawable.approach_white)
                } else if (Integer.parseInt(GeicarData.carData["us_front_left"].toString()) > 50) {
                    findViewById<ImageView>(R.id.image_front_left).setImageResource(R.drawable.approach_green)
                } else if (Integer.parseInt(GeicarData.carData["us_front_left"].toString()) > 30) {
                    findViewById<ImageView>(R.id.image_front_left).setImageResource(R.drawable.approach_orange)
                } else {
                    findViewById<ImageView>(R.id.image_front_left).setImageResource(R.drawable.approach_red)
                }

                if (Integer.parseInt(GeicarData.carData["us_front_center"].toString()) > 70) {
                    findViewById<ImageView>(R.id.image_front_center).setImageResource(R.drawable.approach_white)
                } else if (Integer.parseInt(GeicarData.carData["us_front_center"].toString()) > 50) {
                    findViewById<ImageView>(R.id.image_front_center).setImageResource(R.drawable.approach_green)
                } else if (Integer.parseInt(GeicarData.carData["us_front_center"].toString()) > 30) {
                    findViewById<ImageView>(R.id.image_front_center).setImageResource(R.drawable.approach_orange)
                } else {
                    findViewById<ImageView>(R.id.image_front_center).setImageResource(R.drawable.approach_red)
                }

                if (Integer.parseInt(GeicarData.carData["us_front_right"].toString()) > 70) {
                    findViewById<ImageView>(R.id.image_front_right).setImageResource(R.drawable.approach_white)
                } else if (Integer.parseInt(GeicarData.carData["us_front_right"].toString()) > 50) {
                    findViewById<ImageView>(R.id.image_front_right).setImageResource(R.drawable.approach_green)
                } else if (Integer.parseInt(GeicarData.carData["us_front_right"].toString()) > 30) {
                    findViewById<ImageView>(R.id.image_front_right).setImageResource(R.drawable.approach_orange)
                } else {
                    findViewById<ImageView>(R.id.image_front_right).setImageResource(R.drawable.approach_red)
                }

                if (Integer.parseInt(GeicarData.carData["us_back_left"].toString()) > 70) {
                    findViewById<ImageView>(R.id.image_back_left).setImageResource(R.drawable.approach_white)
                } else if (Integer.parseInt(GeicarData.carData["us_back_left"].toString()) > 50) {
                    findViewById<ImageView>(R.id.image_back_left).setImageResource(R.drawable.approach_green)
                } else if (Integer.parseInt(GeicarData.carData["us_back_left"].toString()) > 30) {
                    findViewById<ImageView>(R.id.image_back_left).setImageResource(R.drawable.approach_orange)
                } else {
                    findViewById<ImageView>(R.id.image_back_left).setImageResource(R.drawable.approach_red)
                }

                if (Integer.parseInt(GeicarData.carData["us_back_center"].toString()) > 70) {
                    findViewById<ImageView>(R.id.image_back_center).setImageResource(R.drawable.approach_white)
                } else if (Integer.parseInt(GeicarData.carData["us_back_center"].toString()) > 50) {
                    findViewById<ImageView>(R.id.image_back_center).setImageResource(R.drawable.approach_green)
                } else if (Integer.parseInt(GeicarData.carData["us_back_center"].toString()) > 30) {
                    findViewById<ImageView>(R.id.image_back_center).setImageResource(R.drawable.approach_orange)
                } else {
                    findViewById<ImageView>(R.id.image_back_center).setImageResource(R.drawable.approach_red)
                }

                if (Integer.parseInt(GeicarData.carData["us_back_right"].toString()) > 70) {
                    findViewById<ImageView>(R.id.image_back_right).setImageResource(R.drawable.approach_white)
                } else if (Integer.parseInt(GeicarData.carData["us_back_right"].toString()) > 50) {
                    findViewById<ImageView>(R.id.image_back_right).setImageResource(R.drawable.approach_green)
                } else if (Integer.parseInt(GeicarData.carData["us_back_right"].toString()) > 30) {
                    findViewById<ImageView>(R.id.image_back_right).setImageResource(R.drawable.approach_orange)
                } else {
                    findViewById<ImageView>(R.id.image_back_right).setImageResource(R.drawable.approach_red)
                }

                findViewById<LinearLayout>(R.id.layoutTrailer).rotation = -GeicarData.carData["trailer_angle"]?.toFloat()!!
                    ?: 0F
                GeicarData.carData["speed"]?.toDouble()?.let {
                    findViewById<ProgressBar>(R.id.progressBar2).setProgress(
                        (2.3 * it.toInt()).toInt(), false)
                }


                if (GeicarData.carData["speed"]?.toDouble()!! < 100 / 2.3 && GeicarData.carData["speed"] != "0.0") {
                    findViewById<TextView>(R.id.speed_text).text = (decimalFormat.format(GeicarData.carData["speed"]?.toDouble()
                        ?.times(2.3) ?: 2.3)).toString()
                } else if (GeicarData.carData["speed"] == "0.0") {
                    findViewById<TextView>(R.id.speed_text).text = "0.0"
                } else {
                    findViewById<TextView>(R.id.speed_text).text = "100"
                }
            }

            delay(100) // Ajoutez un délai d'une seconde entre chaque itération
        }
    }

    private suspend fun fetchData(url: String): Int {
        // Implémentez votre logique d'appel réseau ici
        // Utilisez, par exemple, la bibliothèque Retrofit ou l'API HttpURLConnection
        val url = "http://138.197.181.206/api/allData"
        val httpRequestTask = HttpRequestTask()
        httpRequestTask.execute(url)
        return 0
        // et renvoyez les données que vous souhaitez afficher dans l'interface utilisateur.
        //return "Dummy Data" // Remplacez cela par les données réelles obtenues depuis le réseau.
    }
}

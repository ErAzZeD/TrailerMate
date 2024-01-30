package com.example.myapplication

import HttpRequestTask
import android.annotation.SuppressLint
import android.app.AlertDialog
import android.content.Intent
import android.os.Bundle
import android.util.Log
import android.widget.Button
import android.widget.ImageButton
import android.widget.ImageView
import android.widget.LinearLayout
import android.widget.ProgressBar
import android.widget.TextView
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.lifecycleScope
import com.google.android.gms.maps.CameraUpdateFactory
import com.google.android.gms.maps.GoogleMap
import com.google.android.gms.maps.MapView
import com.google.android.gms.maps.OnMapReadyCallback
import com.google.android.gms.maps.model.LatLng
import com.google.android.gms.maps.model.MarkerOptions
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import java.text.DecimalFormat


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

class MainActivity : AppCompatActivity(), OnMapReadyCallback {

    val decimalFormat = DecimalFormat("#.#")
    private lateinit var mapView: MapView
    private lateinit var googleMap: GoogleMap
    private var isError = false

    @SuppressLint("MissingInflatedId", "SetTextI18n")
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        // mapView = findViewById(R.id.mapView)
        // mapView.onCreate(savedInstanceState)

        // mapView.getMapAsync(this)

        findViewById<TextView>(R.id.textview_us_front_left).text = "COucouuuuuu !!!"
        findViewById<Button>(R.id.button_1).setOnClickListener {
            findViewById<TextView>(R.id.textview_us_front_left).text = "We clicked on the button !!"
        }

        findViewById<ImageButton>(R.id.gpsButtonOpen).setOnClickListener {
            val intent = Intent(this, GPSActivity::class.java)
            startActivity(intent)
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

            Log.d("WORKING IN GPS", "WFSDOP")

            if (GeicarData.carData["server_status"] == "1") {
                if (!isError) {
                    val builder: AlertDialog.Builder = AlertDialog.Builder(this@MainActivity)
                    builder
                        .setMessage("The server is not connected to the application. Please contact TrailerMate.")
                        .setTitle("Server error")

                    val dialog: AlertDialog = builder.create()
                    dialog.show()
                    isError = true

                }
            }

            // Mettez à jour l'interface utilisateur sur le thread principal
            withContext(Dispatchers.Main) {
                findViewById<TextView>(R.id.textview_us_front_left).text = "Front left : " + GeicarData.carData["us_front_left"].toString()
                findViewById<TextView>(R.id.textview_us_front_center).text = "Front center : " + GeicarData.carData["us_front_center"].toString()
                findViewById<TextView>(R.id.textview_us_front_right).text = "Front right : " + GeicarData.carData["us_front_right"].toString()
                findViewById<TextView>(R.id.textview_us_back_left).text = "Back left : " + GeicarData.carData["us_back_left"].toString()
                findViewById<TextView>(R.id.textview_us_back_center).text = "Back center : " + GeicarData.carData["us_back_center"].toString()
                findViewById<TextView>(R.id.textview_us_back_right).text = "Back right : " + GeicarData.carData["us_back_right"].toString()
                findViewById<TextView>(R.id.textview_trailer_angle).text = "Trailer angle : " + GeicarData.carData["trailer_angle"].toString()
                findViewById<TextView>(R.id.textview_speed).text = "Speed : " + GeicarData.carData["speed"].toString()

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

    override fun onMapReady(map: GoogleMap) {
        googleMap = map

        // Position en latitude et longitude
        val position = LatLng(48.8566, 2.3522)

        // Ajoute un marqueur à la position spécifiée
        googleMap.addMarker(MarkerOptions().position(position).title("Marker"))

        // Centre la caméra sur la position spécifiée avec un zoom de 15
        googleMap.moveCamera(CameraUpdateFactory.newLatLngZoom(position, 15f))
    }
}

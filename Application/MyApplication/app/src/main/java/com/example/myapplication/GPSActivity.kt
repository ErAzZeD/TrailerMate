package com.example.myapplication

import HttpRequestTask
import android.annotation.SuppressLint
import android.content.res.Configuration
import android.os.Build
import android.os.Bundle
import android.util.Log
import android.widget.ImageButton
import androidx.annotation.RequiresApi
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.lifecycleScope
import com.google.android.gms.maps.CameraUpdateFactory
import com.google.android.gms.maps.GoogleMap
import com.google.android.gms.maps.MapView
import com.google.android.gms.maps.OnMapReadyCallback
import com.google.android.gms.maps.model.BitmapDescriptorFactory
import com.google.android.gms.maps.model.LatLng
import com.google.android.gms.maps.model.MarkerOptions
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch
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

class GPSActivity : AppCompatActivity(), OnMapReadyCallback {

    val decimalFormat = DecimalFormat("#.#")
    private lateinit var mapView: MapView
    private lateinit var googleMap: GoogleMap

    @RequiresApi(Build.VERSION_CODES.Q)
    @SuppressLint("MissingInflatedId", "SetTextI18n")
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.map_frame)


        mapView = findViewById(R.id.mapView)
        mapView.onCreate(savedInstanceState)

        mapView.getMapAsync(this)

        when (baseContext.resources?.configuration?.uiMode?.and(Configuration.UI_MODE_NIGHT_MASK)) {
            Configuration.UI_MODE_NIGHT_YES -> {mapView.isForceDarkAllowed = true}
            Configuration.UI_MODE_NIGHT_NO -> {mapView.isForceDarkAllowed = false}
            Configuration.UI_MODE_NIGHT_UNDEFINED -> {mapView.isForceDarkAllowed = false}
        }


        findViewById<ImageButton>(R.id.returnButtonGPS).setOnClickListener {
            finish()
        }



    }

    @SuppressLint("SetTextI18n", "CutPasteId")
    private suspend fun loopPrintData() {
        var i = 4
        var position = GeicarData.carData["latitude"]?.toDouble()
            ?.let { GeicarData.carData["longitude"]?.toDouble()?.let { it1 -> LatLng(it, it1) } }!!

        var markerName = googleMap.addMarker(MarkerOptions().position(position).title("Marker"))
        while (true) {
            GeicarData.carData["latitude"]?.let { Log.d("GPS", it) }
            position = GeicarData.carData["latitude"]?.toDouble()
                ?.let { GeicarData.carData["longitude"]?.toDouble()?.let { it1 -> LatLng(it, it1) } }!!
            // Ajoute un marqueur à la position spécifiée

            markerName?.remove()
            markerName = googleMap.addMarker(MarkerOptions().position(position).title("Marker").icon(
                BitmapDescriptorFactory.fromResource(R.drawable.voiture_map)))

            googleMap

            // Centre la caméra sur la position spécifiée avec un zoom de 15

            //googleMap.moveCamera(CameraUpdateFactory.newLatLngZoom(position, 15f))

            delay(1000) // Ajoutez un délai d'une seconde entre chaque itération
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
        val position = LatLng(43.6, 1.4333)

        // Ajoute un marqueur à la position spécifiée
        //googleMap.addMarker(MarkerOptions().position(position).title("Marker"))

        // Centre la caméra sur la position spécifiée avec un zoom de 15
        googleMap.moveCamera(CameraUpdateFactory.newLatLngZoom(position, 12f))

        lifecycleScope.launch {
            loopPrintData()
        }
    }
}


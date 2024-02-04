
import android.os.AsyncTask
import android.util.Log
import com.example.myapplication.GeicarData
import org.json.JSONArray
import org.json.JSONObject
import java.io.BufferedReader
import java.io.InputStreamReader
import java.net.HttpURLConnection
import java.net.URL


class HttpRequestTask : AsyncTask<String, Void, String>() {

    @Deprecated("Deprecated in Java")
    override fun doInBackground(vararg params: String): String {
        var result = ""
        val urlString = params[0]

        try {
            val url = URL(urlString)
            val connection = url.openConnection() as HttpURLConnection

            // Configurer la requête
            connection.requestMethod = "GET"

            // Lire la réponse
            val reader = BufferedReader(InputStreamReader(connection.inputStream))
            var line: String?

            while (reader.readLine().also { line = it } != null) {
                result += line
            }

            // Fermer la connexion
            connection.disconnect()

        } catch (e: Exception) {
            e.printStackTrace()
            result = "Erreur : ${e.message}"
        }

        return result
    }

    @Deprecated("Deprecated in Java")
    override fun onPostExecute(result: String) {
        // Traitement du résultat
        // Ici, vous pouvez mettre à jour l'interface utilisateur ou effectuer d'autres actions nécessaires.
        Log.d("AH", result);

        val obj = JSONObject(result).get("data")
        var ret = ""
        if (obj::class.simpleName != null) {

            ret = obj::class.simpleName.toString()

        }

        if (obj is JSONArray) {
            for (i in 0..<obj.length()) {
                val itemTemp = obj.getJSONObject(i)
                val key = listOf(itemTemp.keys())[0].next().toString()
                val value = itemTemp.get(key)
                GeicarData.carData[key] = value.toString()
                ret = value.toString()
            }
            //ret = obj[1]::class.simpleName.toString()
        }


        Log.d("VALUE OF FRONT LEFT", ret);
    }
}
import android.os.AsyncTask
import android.util.Log
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.withContext
import org.json.JSONObject
import java.io.BufferedOutputStream
import java.io.OutputStreamWriter
import java.net.HttpURLConnection
import java.net.URL



class HttpSendTask  {

    suspend fun sendData(vararg params: Pair<String, JSONObject>): String {
        var result = ""
        val urlStr = params[0].first
        val data = params[0].second

        return withContext(Dispatchers.IO) {
            try {
                val url = URL(urlStr)
                val connection = url.openConnection() as HttpURLConnection

                // Configurer la requête
                connection.requestMethod = "PUT"
                connection.doOutput = true
                connection.setRequestProperty("Content-Type", "application/json")

                // Écrire les données à envoyer
                val outputStream = BufferedOutputStream(connection.outputStream)
                val writer = OutputStreamWriter(outputStream)
                writer.write(data.toString())
                writer.flush()

                // Lire la réponse
                val reader = connection.inputStream.bufferedReader()
                var line: String?

                while (reader.readLine().also { line = it } != null) {
                    result += line
                }

                // Fermer la connexion
                outputStream.close()
                connection.disconnect()

                result
            } catch (e: Exception) {
                e.printStackTrace()
                "Erreur : ${e.message}"
            }
        }
    }


}

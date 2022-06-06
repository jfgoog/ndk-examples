package com.example.hackernews

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.TextView
import com.example.hackernews.databinding.ActivityMainBinding
import java.io.FileOutputStream

class MainActivity : AppCompatActivity() {
    private val cacert by lazy {
        val path = cacheDir.resolve("cacert.pem")
        assets.open("cacert.pem").copyTo(FileOutputStream(path))
        path
    }

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Example of a call to a native method
        binding.sampleText.text = getHackerNews(cacert.path)
    }

    /**
     * A native method that is implemented by the 'hackernews' native library,
     * which is packaged with this application.
     */
    external fun getHackerNews(cacert: String): String

    companion object {
        // Used to load the 'hackernews' library on application startup.
        init {
            System.loadLibrary("hackernews")
        }
    }
}
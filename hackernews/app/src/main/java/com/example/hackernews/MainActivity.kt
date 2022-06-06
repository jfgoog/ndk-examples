package com.example.hackernews

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import androidx.recyclerview.widget.RecyclerView
import java.io.FileOutputStream

class MainActivity : AppCompatActivity() {
    private val cacert by lazy {
        val path = cacheDir.resolve("cacert.pem")
        assets.open("cacert.pem").copyTo(FileOutputStream(path))
        path
    }

    private lateinit var adapter: NewsAdapter

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        adapter = NewsAdapter(getHackerNews(cacert.path))

        val recyclerView: RecyclerView = findViewById(R.id.recycler_view)
        recyclerView.adapter = adapter
    }

    /**
     * A native method that is implemented by the 'hackernews' native library,
     * which is packaged with this application.
     */
    external fun getHackerNews(cacert: String): Array<String>

    companion object {
        // Used to load the 'hackernews' library on application startup.
        init {
            System.loadLibrary("hackernews")
        }
    }
}
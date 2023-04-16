package com.example.ciclo

import android.content.Intent
import android.graphics.Bitmap
import android.media.ExifInterface
import android.os.Bundle
import android.os.Environment
import android.provider.MediaStore
import android.widget.Button
import android.widget.ImageView
import android.widget.LinearLayout
import android.widget.TextView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import java.io.File
import java.io.FileOutputStream
import java.text.SimpleDateFormat
import java.util.*

class MainActivity : AppCompatActivity() {

    private lateinit var imageView: ImageView
    private lateinit var button: Button
    private lateinit var linearLayout: LinearLayout
    private val REQUEST_IMAGE_CAPTURE = 100

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        imageView = findViewById(R.id.Image_Car)
        button = findViewById(R.id.btn_take_pic)
        linearLayout = findViewById(R.id.linear_layout)

        button.setOnClickListener {
            dispatchTakePictureIntent()
        }
    }

    private fun dispatchTakePictureIntent() {
        val takePictureIntent = Intent(MediaStore.ACTION_IMAGE_CAPTURE)
        if (takePictureIntent.resolveActivity(packageManager) != null) {
            startActivityForResult(takePictureIntent, REQUEST_IMAGE_CAPTURE)
        } else {
            Toast.makeText(this, "No camera app found", Toast.LENGTH_SHORT).show()
        }
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)

        if (requestCode == REQUEST_IMAGE_CAPTURE && resultCode == RESULT_OK) {
            val imageBitmap = data?.extras?.get("data") as Bitmap
            imageView.setImageBitmap(imageBitmap)
            saveImage(imageBitmap)
        } else {
            Toast.makeText(this, "Error taking photo", Toast.LENGTH_SHORT).show()
        }
    }

    private fun saveImage(bitmap: Bitmap?) {
        val imageFileName = "JPEG_" + SimpleDateFormat("yyyyMMdd_HHmmss", Locale.getDefault()).format(Date()) + "_"
        val storageDir = getExternalFilesDir(Environment.DIRECTORY_PICTURES)
        val imageFile = File.createTempFile(
            imageFileName,
            ".jpg",
            storageDir
        )

        val outputStream = FileOutputStream(imageFile)
        bitmap?.compress(Bitmap.CompressFormat.JPEG, 100, outputStream)
        outputStream.flush()
        outputStream.close()

        // Extract time and date from image metadata
        val exif = ExifInterface(imageFile.absolutePath)
        var dateTime = exif.getAttribute(ExifInterface.TAG_DATETIME)
        if (dateTime == null) {
            // Use current date and time as a fallback
            dateTime = SimpleDateFormat("dd-MM-yyyy HH:mm", Locale.getDefault()).format(Date())
        }

        // Add the image to the linear layout with the time and date information
        val image = ImageView(this)
        image.setImageBitmap(bitmap)

        val textView = TextView(this)
        textView.text = "$dateTime"
        linearLayout.addView(textView)
        linearLayout.addView(image)

        Toast.makeText(this, "Image saved to: ${imageFile.absolutePath}", Toast.LENGTH_SHORT).show()
    }
}

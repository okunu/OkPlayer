package com.ou.demo.video

import android.Manifest
import android.annotation.SuppressLint
import android.content.Context
import android.content.pm.PackageManager
import android.graphics.ImageFormat
import android.graphics.SurfaceTexture
import android.hardware.camera2.*
import android.media.ImageReader
import android.os.Bundle
import android.util.Log
import android.util.Size
import android.util.SparseIntArray
import android.view.Surface
import android.view.TextureView
import android.widget.ImageView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.core.app.ActivityCompat
import com.ou.demo.databinding.ActivityVideoBinding
import java.util.*
import kotlin.math.abs


class VideoActivity: AppCompatActivity() {

    val REQUEST_CODE = 1026
    lateinit var binding: ActivityVideoBinding

    var mCameraManager: CameraManager? = null
    var mFrontCameraId: String? = null
    var mFrontCameraChar: CameraCharacteristics? = null
    var mBackCameraId: String? = null
    var mBackCameraChar: CameraCharacteristics? = null
    var mImageRender: ImageReader? = null
    var mCameraDevice: CameraDevice? = null
    var mCameraSession: CameraCaptureSession? = null
    var mTextureView: TextureView? = null
    var mImageView: ImageView? = null
    var mSensorOri: Int? = null
    var mCameraId: String? = null

    var orientations: SparseIntArray = SparseIntArray()

    init {
        orientations.append(Surface.ROTATION_0, 90);
        orientations.append(Surface.ROTATION_90, 0);
        orientations.append(Surface.ROTATION_180, 270);
        orientations.append(Surface.ROTATION_270, 180);
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityVideoBinding.inflate(layoutInflater)
        setContentView(binding.root)
        requestPermission()
        initCamera()
        mTextureView = binding.surface
        mImageView = binding.image
        binding.switchBtn.setOnClickListener { switchCamera() }
        binding.takePicBtn.setOnClickListener { takePhoto() }
    }

    override fun onResume() {
        super.onResume()
        mTextureView?.let {
            if (it.width != 0) {
                openCamera(it.width, it.height)
            } else {
                it.surfaceTextureListener = PreviewCallback()
            }
        }
    }

    override fun onPause() {
        super.onPause()
        closeCamera()
    }

    fun closeCamera() {
        mCameraSession?.let {
            try {
                it.stopRepeating()
                mCameraSession = null
            } catch (e: Exception) {
            }
        }
        mCameraDevice?.let {
            it.close()
            mCameraDevice = null
        }
        mImageRender?.let {
            it.close()
            mImageRender = null
        }
    }

    @SuppressLint("MissingPermission")
    fun openCamera(width: Int, height: Int){
        var characteristics = if (mCameraId == mBackCameraId) mBackCameraChar else mFrontCameraChar
        var map = characteristics!!.get(CameraCharacteristics.SCALER_STREAM_CONFIGURATION_MAP)
        mSensorOri = characteristics.get(CameraCharacteristics.SENSOR_ORIENTATION)
        var previewSizes = map!!.getOutputSizes(SurfaceTexture::class.java)
        var bestSize = getBestSize(width, height, previewSizes)

        mTextureView!!.surfaceTexture!!.setDefaultBufferSize(bestSize!!.width, bestSize.height)

        var sizes = map.getOutputSizes(ImageFormat.JPEG)

        var large = sizes[0]
        for (temp in sizes) {
            if (temp.width * temp.height > large.width * large.height) {
                large = temp
            }
        }

        mImageRender = ImageReader.newInstance(large.width, large.height, ImageFormat.JPEG, 1)
        mImageRender!!.setOnImageAvailableListener(ImageAvailable(), null)

        try {
            mCameraManager!!.openCamera(mCameraId!!, CameraDeviceCallback(), null)
        } catch (e: java.lang.Exception) {

        }
    }

    fun getBestSize(shortSize: Int, longSize: Int, sizes: Array<Size>): Size? {
        var bestSize: Size? = null
        var uiRatio = longSize / shortSize
        var minRatio = uiRatio
        for (size in sizes) {
            var cameraRatio = size.width / size.height

            var offset = abs(cameraRatio - minRatio)
            if (offset < minRatio) {
                minRatio = offset
                bestSize = size
            }
            if (uiRatio == cameraRatio) {
                bestSize = size
                break
            }
        }
        return bestSize
    }

    fun initCamera() {
        try {
            mCameraManager = getSystemService(Context.CAMERA_SERVICE) as CameraManager?
            mCameraManager?.let {
                var idList = it.cameraIdList
                for (id in idList) {
                    var characteristics = it.getCameraCharacteristics(id)
                    var faing = characteristics.get(CameraCharacteristics.LENS_FACING)
                    if (faing != null) {
                        if (faing == CameraCharacteristics.LENS_FACING_BACK){
                            mBackCameraId = id
                            mBackCameraChar = characteristics
                        } else if (faing == CameraCharacteristics.LENS_FACING_FRONT){
                            mFrontCameraId = id
                            mFrontCameraChar = characteristics
                        }
                        mCameraId = id
                    }
                    var level = characteristics.get(CameraCharacteristics.INFO_SUPPORTED_HARDWARE_LEVEL)
                    if (level == null || level == CameraCharacteristics.INFO_SUPPORTED_HARDWARE_LEVEL_LEGACY){
                        break
                    }
                }
            }
        } catch (e: java.lang.Exception) {
            Log.e("okunu", "init", e)
        }
    }

    fun switchCamera() {
        mCameraId = if (mCameraId == mBackCameraId) mFrontCameraId else mBackCameraId
        closeCamera()
        openCamera(mTextureView!!.width, mTextureView!!.height)
    }

    fun getOrientation(ori: Int) : Int{
        return (orientations.get(ori) + mSensorOri!! + 270) % 360
    }

    fun takePhoto() {
        val request = mCameraDevice!!.createCaptureRequest(CameraDevice.TEMPLATE_STILL_CAPTURE)
        request.addTarget(mImageRender!!.surface)
        request.set(CaptureRequest.CONTROL_AF_MODE,
            CaptureRequest.CONTROL_AF_MODE_CONTINUOUS_PICTURE)
        request.set(CaptureRequest.CONTROL_AF_MODE,
            CaptureRequest.CONTROL_AE_MODE_ON_AUTO_FLASH)
        val rotation = windowManager.defaultDisplay.rotation
        request.set(CaptureRequest.JPEG_ORIENTATION, getOrientation(rotation))
        mCameraSession!!.stopRepeating()

        mCameraSession!!.capture(request.build(), object : CameraCaptureSession.CaptureCallback() {
            override fun onCaptureCompleted(session: CameraCaptureSession, request: CaptureRequest, result: TotalCaptureResult) {
                super.onCaptureCompleted(session, request, result)
                try {
                    val request1 = mCameraDevice!!.createCaptureRequest(CameraDevice.TEMPLATE_PREVIEW)
                    request1.addTarget(Surface(mTextureView!!.surfaceTexture))
                    mCameraSession!!.setRepeatingRequest(request1.build(), null, null)
                }catch (e: Exception) {

                }
            }
        }, null)
    }

    fun requestPermission() {
        var hasPermission = ActivityCompat.checkSelfPermission(this,
            Manifest.permission.CAMERA) == PackageManager.PERMISSION_GRANTED
        if (!hasPermission) {
            ActivityCompat.requestPermissions(this,
                arrayOf(Manifest.permission.CAMERA), REQUEST_CODE)
        }
    }

    override fun onRequestPermissionsResult(requestCode: Int, permissions: Array<out String>, grantResults: IntArray) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        if (requestCode == REQUEST_CODE) {
            var index = permissions.indexOf(Manifest.permission.CAMERA)
            var result = grantResults[index]
            if (result != PackageManager.PERMISSION_GRANTED) {
                Toast.makeText(this, "has no permission", Toast.LENGTH_SHORT).show()
            }
        }
    }

    fun createPreviewPipeline(camera: CameraDevice) {
        try {
            var captureBuilder = camera.createCaptureRequest(CameraDevice.TEMPLATE_PREVIEW)
            var suface = Surface(mTextureView!!.surfaceTexture)
            captureBuilder.addTarget(suface)
            camera.createCaptureSession(Arrays.asList(suface, mImageRender!!.surface), object : CameraCaptureSession.StateCallback() {
                override fun onConfigured(session: CameraCaptureSession) {
                    mCameraSession = session
                    try {
                        captureBuilder.set(CaptureRequest.CONTROL_AE_MODE,CaptureRequest.CONTROL_AF_MODE_CONTINUOUS_PICTURE)
                        captureBuilder.set(CaptureRequest.CONTROL_AE_MODE,CaptureRequest.CONTROL_AE_MODE_ON_AUTO_FLASH)
                        val build = captureBuilder.build()
                        session.setRepeatingRequest(build,null,null);
                    }catch (e: Exception) {
                        Log.e("okunu", "onConfigured", e)
                    }
                }

                override fun onConfigureFailed(session: CameraCaptureSession) {
                    Log.i("okunu", "onConfigureFailed")
                }

            }, null)
        } catch (e: Exception) {
            Log.e("okunu", "createPreviewPipeline", e)
        }
    }

    inner class CameraDeviceCallback: CameraDevice.StateCallback() {
        override fun onOpened(camera: CameraDevice) {
            mCameraDevice = camera
            createPreviewPipeline(camera)
        }

        override fun onDisconnected(camera: CameraDevice) {
            camera.close()
            mCameraDevice == null
        }

        override fun onError(camera: CameraDevice, error: Int) {
            camera.close()
            mCameraDevice = null
        }

    }

    inner class PreviewCallback: TextureView.SurfaceTextureListener {
        override fun onSurfaceTextureAvailable(surface: SurfaceTexture, width: Int, height: Int) {
            openCamera(width, height)
        }

        override fun onSurfaceTextureSizeChanged(surface: SurfaceTexture, width: Int, height: Int) {

        }

        override fun onSurfaceTextureDestroyed(surface: SurfaceTexture): Boolean {
            return false
        }

        override fun onSurfaceTextureUpdated(surface: SurfaceTexture) {

        }

    }

    class ImageAvailable : ImageReader.OnImageAvailableListener {
        override fun onImageAvailable(reader: ImageReader?) {
            Log.i("okunu", "pic is available")
        }
    }

    class CompareSizesByArea: Comparator<Any> {
        override fun compare(lhs: Any, rhs: Any): Int {
            if (lhs is Size && rhs is Size) {
                return lhs.width * lhs.height - rhs.width * rhs.height
            }
            return 0
        }
    }
}
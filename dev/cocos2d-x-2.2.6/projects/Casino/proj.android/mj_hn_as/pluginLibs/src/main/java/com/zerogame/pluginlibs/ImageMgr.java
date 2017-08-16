package com.zerogame.pluginlibs;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.content.DialogInterface;
import android.content.Intent;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.widget.Toast;

import com.zerogame.pluginlibs.notification.AlarmNotification;

public class ImageMgr {

	public static final int FILECHOOSER_RESULTCODE = 11111;
	public static final int REQ_CAMERA = FILECHOOSER_RESULTCODE + 1;
	public static final int REQ_CHOOSE = REQ_CAMERA + 1;

	private static ImageMgr instance;
	private static Activity mainActivity;
	private static int size = 100;

	public static ImageMgr getInstance() {
		if (instance == null) {
			instance = new ImageMgr();
		}
		return instance;
	}

	public void configWithAcitivity(Activity activity) {
		mainActivity = activity;
		AlarmNotification.Ince().setActivity(activity);
	}

	public void selectImageResult(int requestCode, int resultCode, Intent data) {
		if (requestCode == REQ_CAMERA) {
			Uri uri = afterOpenCamera();
			if (uri != null) {
				if (size <= 0) {
					Bitmap bitmap = null;
					try {
						bitmap = MediaStore.Images.Media.getBitmap(
								mainActivity.getContentResolver(), uri);
					} catch (Exception e) {
						e.printStackTrace();
					}
					if (bitmap != null)
						notifyToView(bitmap);
				} else {
					startPhotoZoom(uri);
				}
			}
		} else if (requestCode == REQ_CHOOSE) {
			if (data != null) {
				Uri uri = afterChosePic(data);
				if (size <= 0) {
					Bitmap bitmap = null;
					try {
						bitmap = MediaStore.Images.Media.getBitmap(
								mainActivity.getContentResolver(), uri);
					} catch (Exception e) {
						e.printStackTrace();
					}
					if (bitmap != null)
						notifyToView(bitmap);
				} else {
					startPhotoZoom(uri);
				}
			}
		} else if (requestCode == FILECHOOSER_RESULTCODE) {
			if (data != null)
				setPicToView(data);
		}

	}

	private void setPicToView(Intent picdata) {
		Bundle extras = picdata.getExtras();
		if (extras != null) {
			Bitmap temp = extras.getParcelable("data");
			notifyToView(temp);
		}
	}

	private void notifyToView(Bitmap temp) {
		Bitmap photo = temp;
		if (size > 0) {
			photo = Bitmap.createScaledBitmap(temp, size, size, true);
			temp.recycle();
			temp = null;
		}

		try {
			final String path = Environment.getExternalStorageDirectory()
					.getPath()
					+ "/fuiou_wmp/temp/"
					+ (System.currentTimeMillis() + ".png");
			photo.compress(Bitmap.CompressFormat.PNG, 60, new FileOutputStream(
					path, false));
			photo.recycle();
			photo = null;
			mainActivity.runOnUiThread(new Runnable() {
				@Override
				public void run() {
					AppPlugin.onNotifySelectImage(path);
				}
			});
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
	}

	public void startPhotoZoom(Uri uri) {
		Intent intent = new Intent("com.android.camera.action.CROP");
		intent.setDataAndType(uri, "image/*");
		if (size <= 0) {
			// try {
			// Bitmap bitmap =
			// MediaStore.Images.Media.getBitmap(mainActivity.getContentResolver(),
			// uri);
			// intent.putExtra("outputX", bitmap.getWidth());
			// intent.putExtra("outputY", bitmap.getHeight());
			// } catch (IOException e) {
			// e.printStackTrace();
			// }
		} else {
			intent.putExtra("aspectX", 1);
			intent.putExtra("aspectY", 1);
			intent.putExtra("outputY", size);
		}
		intent.putExtra("return-data", true);
		mainActivity.startActivityForResult(intent, FILECHOOSER_RESULTCODE);
	}

	public void selectImage(int clipWidth) {
		size = clipWidth;
		String[] selectPicTypeStr = {
				mainActivity.getResources().getString(
						R.string.icon_loading_photo),
				mainActivity.getResources()
						.getString(R.string.icon_loading_img) };
		AlertDialog.Builder builder = new Builder(mainActivity);
		builder.setItems(selectPicTypeStr,
				new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int which) {
						switch (which) {
						// 相机拍摄
						case 0:
							openCarcme();
							break;
						// 手机相册
						case 1:
							chosePic();
							break;
						default:
							break;
						}
					}
				});
		builder.show();
	}

	static String imagePaths;
	static Uri cameraUri;

	/**
	 * 打开照相
	 */
	private void openCarcme() {

		imagePaths = Environment.getExternalStorageDirectory().getPath()
				+ "/fuiou_wmp/temp/head_"
				+ (System.currentTimeMillis() + ".png");

		cameraUri = Uri.fromFile(fileExtst(imagePaths));
		Intent intent = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
		intent.putExtra(MediaStore.EXTRA_OUTPUT, cameraUri);
		mainActivity.startActivityForResult(intent, REQ_CAMERA);
	}

	/**
	 * 拍照结束
	 */
	private Uri afterOpenCamera() {
		String path = Environment.getExternalStorageDirectory().getPath()
				+ "/fuiou_wmp/temp/head_"
				+ (System.currentTimeMillis() + ".png");
		File vFile = new File(imagePaths);
		if (vFile.exists()) {
			File newFile = FileUtils.compressFile(imagePaths, path);
			return Uri.fromFile(newFile);
		} else {
			return null;
		}
	}

	/**
	 * 本地相册选择图片
	 */
	private void chosePic() {
		Intent intent = new Intent(Intent.ACTION_PICK,
				android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
		mainActivity.startActivityForResult(intent, REQ_CHOOSE);
	}

	/**
	 * 选择照片
	 * 
	 * @param data
	 */
	public Uri afterChosePic(Intent data) {
		// 获取图片的路径：
		String[] proj = { MediaStore.Images.Media.DATA };
		// 好像是android多媒体数据库的封装接口，具体的看Android文档
		Uri theUri = data.getData();
		if (theUri == null) {
			return null;
		}
		Cursor cursor = mainActivity.managedQuery(theUri, proj, null,
				null, null);
		if (cursor == null) {
			Toast.makeText(
					mainActivity,
					mainActivity.getResources().getString(
							R.string.icon_loading_tip), Toast.LENGTH_SHORT)
					.show();
			return null;
		}
		int column_index = cursor
				.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);
		cursor.moveToFirst();
		String path = cursor.getString(column_index);

		String imagePaths = Environment.getExternalStorageDirectory().getPath()
				+ "/fuiou_wmp/temp/" + (System.currentTimeMillis() + ".png");
		// 必须确保文件夹路径存在，否则拍照后无法完成回�?
		fileExtst(imagePaths);
		Uri uri = Uri.fromFile(FileUtils.compressFile(path, imagePaths));
		return uri;
	}

	static public File fileExtst(String path) {
		File vFile = new File(path);
		if (!vFile.exists()) {
			File vDirPath = vFile.getParentFile();
			vDirPath.mkdirs();
		}
		return vFile;
	}

}
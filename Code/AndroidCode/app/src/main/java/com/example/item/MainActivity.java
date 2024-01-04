package com.example.item;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;

public class MainActivity extends AppCompatActivity {
    private Button Off_Btn;
    private Button On_Btn;
    private String Off_http = "http://192.168.1.104/gpio/1";
    private String On_http = "http://192.168.1.104/gpio/0";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Off_Btn = findViewById(R.id.On_light);
        On_Btn = findViewById(R.id.Off_light);
        Off_Btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                On_Light();
            }
        });

        On_Btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Off_light();
            }
        });

    }


    public void On_Light(){

        getDatasync(On_http);
    }

    public void Off_light(){
        getDatasync(Off_http);
    }

    private  String TAG = "Set";
    public void getDatasync(final String httpfor){
        Log.e(TAG, "getDatasync: 发送 = "+ httpfor);
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    OkHttpClient client = new OkHttpClient();//创建OkHttpClient对象
                    Request request = new Request.Builder()
                            .url(httpfor)//请求接口。如果需要传参拼接到接口后面。
                            .build();//创建Request 对象
                    Response response = null;
                    response = client.newCall(request).execute();//得到Response 对象
                    if (response.isSuccessful()) {

                        //此时的代码执行在子线程，修改UI的操作请使用handler跳转到UI线程。
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }).start();
    }
}

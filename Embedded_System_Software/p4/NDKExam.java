package org.example.ndk;





import android.app.Activity;
import android.graphics.Color;
import android.graphics.drawable.GradientDrawable;
import android.os.Bundle;
import android.os.Handler;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnTouchListener;
import android.view.ViewGroup.LayoutParams;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;

public class NDKExam extends Activity {
	LinearLayout linear;
	int rowSize = 5, colSize = 4;
	int[] value = new int[rowSize * colSize];
	int[] btnId = new int[rowSize * colSize];
	Button[] buttons = new Button[rowSize * colSize];
	int Click1 = -1;
	int Click2 = -1;
	int answer = 0;
	int dev;
	boolean startGame = false;
	//variables for class
	public native int getScore(int dev);
	public native int startGame(int dev);
	public native int finishGame(int dev);
	public native int driveOpen();
	//declaration for JNI
	protected void assignNum()
	{
		int totalnum = rowSize * colSize / 2;
		for(int i = 0; i < totalnum; i++)
		{
			value[i] = i;
			value[i+totalnum] = i;
		}
		for(int i = 0; i < value.length; i++)
		{
			int j = (int)(Math.random()*value.length);
			int tmp = 0;
			tmp = value[i];
			value[i] = value[j];
			value[j] = tmp;
			
		}
	}
	//randomly distribute numbers to buttons.
	@Override
	protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        System.loadLibrary("puzzle-game");
        //load JNI library
        setContentView(R.layout.activity_main);
        linear = (LinearLayout)findViewById(R.id.container);
        final Button startButton = (Button)findViewById(R.id.start);
        dev = driveOpen();
        //JNI function for opening device driver
        //driver opening only once at creation
        startButton.setText("START");
        OnClickListener listener=new OnClickListener(){
			public void onClick(View v){
				assignNum();
				for(int i = 0; i < value.length; i++){
					buttons[i].setText(Integer.toString(value[i]));
				}
				startButton.setText("RESET");
				TextView tv = (TextView)findViewById(R.id.textView1);
				tv.setText("Lets Play!");
				Handler handler = new Handler();
				handler.postDelayed(new Runnable(){
					public void run(){
						for(int i = 0; i < value.length; i++){
							buttons[i].setText("");}
						startGame = true;
						answer = 0;
						
						startGame(dev);
					}
				},3000);
			}
		};
		//onClickListener for start button
		//if the game is started, distribute numbers to button and set basic variables.
		startButton.setOnClickListener(listener);
        GradientDrawable drawable = new GradientDrawable();
        drawable.setShape(GradientDrawable.RECTANGLE);
        drawable.setStroke(5, Color.GREEN);
        drawable.setColor(Color.CYAN);
        int count = 0;
        
        //onClickListener for puzzles.
        //it compares view's id and button's id to match touched button and real one.
        //basic algorithm for puzzle game is in this listener.
        OnClickListener buttonLis = new OnClickListener(){
        	public void onClick(View v)
        	{
        		
        		if(startGame) //only works when the game is started
        		{

        			if(Click1 == -1) // when none of card is chosen, pick touched card
        			{
        				if(buttons[v.getId()].getText() == ""){
        					buttons[v.getId()].setText(Integer.toString(value[v.getId()]));
        					Click1 = v.getId();}
        				
        			}
        			else if(Click2 == -1) //when 2 cards were chosen, compare it
        			{
        				Click2 = v.getId();
        				if(buttons[v.getId()].getText() == ""){
        					buttons[v.getId()].setText(Integer.toString(value[v.getId()]));
	        				if(value[Click1] == value[Click2])
	        				{
	        					//if two cards are matched, call JNI function to give score to device
	        					getScore(dev);
	        					Click1 = -1;
	        					Click2 = -1;
	        					answer += 1;
	        				}
	        				else
	        				{
	        				
	        					Handler handler2 = new Handler();
	        					handler2.postDelayed(new Runnable(){
	        						public void run(){
	        							buttons[Click1].setText("");
	    	        					buttons[Click2].setText("");
	    	        					Click1 = -1;
	    	        					Click2 = -1;
	        						}
	        					},500);
	        					
	        				}
        				}
        				else if(Click1 == Click2)
        				{
        					System.out.println("value : " + value[v.getId()]);
        					buttons[v.getId()].setText("");
        					Click1 = -1;
        					Click2 = -1;
        					
        				}
        				else
        				{
        					Click2 = -1;
        				
        				}
        			}
        			
        		}
        		if(answer == colSize*rowSize/2)
        		{
        			if(finishGame(dev) == 1) // when pairs are found, call JNI function to finish timer.
        			{
        				TextView tv = (TextView)findViewById(R.id.textView1);
        				tv.setText("Contratulation! You got New Record!");
        			}
        			else
        			{
        				TextView tv = (TextView)findViewById(R.id.textView1);
        				tv.setText("Good Job!");
        			
        			}
        			startGame = false;
        		}
        	}
        };
        for(int i = 0; i < rowSize; i++)
        {
        	LinearLayout row = new LinearLayout(this);
        	row.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.WRAP_CONTENT));
        	for(int j = 0; j < colSize; j++)
        	{
        		buttons[count] = new Button(this);
        		//buttons[count].setLayoutParams(new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT));
        		LinearLayout.LayoutParams p = new LinearLayout.LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
        		p.leftMargin = 10;
        		p.bottomMargin = 10;
        		buttons[count].setLayoutParams(p);
        		buttons[count].setWidth(70);
        		buttons[count].setHeight(70);
        		buttons[count].setBackgroundDrawable(drawable);
        		buttons[count].setId(count);
        		buttons[count].setOnClickListener(buttonLis);
        		row.addView(buttons[count]);
        		btnId[count] = buttons[count].getId();
        		count+=1;
        	}
        	
        	linear.addView(row);
        	
        }
        
        //dynamically make buttons as a card.
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

}

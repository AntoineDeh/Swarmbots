package project.src.view.ressources;

import static java.lang.Math.*;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Point;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;

import project.src.view.viewmodel.CommandScreenViewModel;

public class JoystickView extends View implements View.OnTouchListener {

    private CommandScreenViewModel model;
    private Paint outerCirclePaint, innerCirclePaint;
    private Point center;
    private int outerRadius, innerRadius;
    private float innerX, innerY;
    private JoystickListener listener;

    // public MutableLiveData<Float> ;

    public JoystickView(Context context, AttributeSet attrs) {
        super(context, attrs);
        setOnTouchListener(this);

        //

        // Initialise les peintures
        outerCirclePaint = new Paint();
        outerCirclePaint.setColor(Color.GRAY);
        outerCirclePaint.setStyle(Paint.Style.STROKE);
        outerCirclePaint.setStrokeWidth(10);

        innerCirclePaint = new Paint();
        innerCirclePaint.setColor(Color.rgb(117,156,255));
        innerCirclePaint.setStyle(Paint.Style.FILL);

        // Initialise les dimensions
        center = new Point(0, 0);
        outerRadius = 130;
        innerRadius = 75;
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        // Dessine le cercle extérieur
        canvas.drawCircle(center.x, center.y, outerRadius, outerCirclePaint);
        // Dessine le cercle intérieur
        canvas.drawCircle(innerX, innerY, innerRadius, innerCirclePaint);
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(w, h, oldw, oldh);
        // Définit le centre du joystick
        center.x = w / 2;
        center.y = h / 2;
        // Initialise les positions du cercle intérieur au centre
        innerX = center.x;
        innerY = center.y;
    }

    @Override
    public boolean onTouch(View view, MotionEvent event) {
        float distance;
        if (event.getAction() == MotionEvent.ACTION_UP){
            innerX = center.x;
            innerY = center.y;
            invalidate();
            if (listener != null) {
                float deltaX = (innerX - center.x) / outerRadius;
                float deltaY = (innerY - center.y) / outerRadius;
                listener.onJoystickMoved(deltaX, deltaY);
            }
            return true;
        }
        // Calcule la distance entre le toucher et le centre du joystick
        distance = (float) sqrt(pow(event.getX() - center.x, 2) + pow(event.getY() - center.y, 2));
        // Si le toucher est dans le cercle extérieur
        if (distance <= outerRadius) {
            // Met à jour les positions du cercle intérieur
            innerX = event.getX();
            innerY = event.getY();
            // Redessine la vue
            invalidate();
            // Informe l'écouteur du joystick
            if (listener != null) {
                float deltaX = (innerX - center.x) / outerRadius;
                float deltaY = (innerY - center.y) / outerRadius;
                listener.onJoystickMoved(deltaX, deltaY);
                if(-Math.cos(45)*outerRadius<deltaX && deltaX<Math.cos(45)*outerRadius &&
                        deltaY>Math.sin(45)*outerRadius){
                    // Avancer
                } else if(-Math.cos(45)*outerRadius<deltaX && deltaX<Math.cos(45)*outerRadius &&
                        deltaY<-Math.sin(45)*outerRadius) {
                    // Reculer
                } else if(deltaX>Math.cos(45)*outerRadius && deltaY<Math.sin(45)*outerRadius &&
                        deltaY>-Math.sin(45)*outerRadius) {
                    // Droite
                } else if(deltaX<-Math.cos(45)*outerRadius && deltaY<Math.sin(45)*outerRadius &&
                        deltaY>-Math.sin(45)*outerRadius) {
                    // Gauche
                }
            }
            return true;
        }
        return false;
    }

    public void setJoystickListener(JoystickListener listener) {
        this.listener = listener;
    }

    public interface JoystickListener {
        void onJoystickMoved(float deltaX, float deltaY);
    }
}


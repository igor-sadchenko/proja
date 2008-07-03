using System;
using System.Collections.Generic;
using System.Text;
using System.Windows;

namespace NUIGroup.MultiTouchFramework
{
    #region touchdata class
    /// <summary>
    /// The .NET version of the Touchlib TouchData struct
    /// </summary>
    public class TouchData
    {
        //_touchedElement is the object that this finger touched
        private UIElement _touchedElement;

        private int _ID;
        private int _tagID;
        private float _X;
        private float _Y;
        private float _angle;
        private float _area;
        private float _height;
        private float _width;
        private float _dX;
        private float _dY;


        public TouchData(UIElement elem,int ID, int tagID, float X, float Y, float angle, float area, float height, float width, float dX, float dY)
        {
            this._touchedElement = elem;
            this._ID = ID;
            this._tagID = tagID;
            this._X = X;
            this._Y = Y;
            this._angle = angle;
            this._area = area;
            this._height = height;
            this._width = width;
            this._dX = dX;
            this._dY = dY;
        }

        #region Accessor Methods
        public UIElement touchedElement
        {
            get { return _touchedElement; }
        }
        public int ID
        {
            get { return _ID; }
        }
        public int tagID
        {
            get { return _tagID; }
        }
        public float X 
        {
            get { return _X; }
            
        }
        public float Y
        {
            get { return _Y; }
        }
        public float angle
        {
            get { return _angle; }
        }
        public float area
        {
            get { return _area; }
        }
        public float height
        {
            get { return _height; }
        }
        public float width
        {
            get { return _width; }
        }
        public float dX
        {
            get { return _dX; }
        }
        public float dY
        {
            get { return _dY; }
        }
        #endregion
        //Mutator

        public void setX(float value)
        {
            this._X = value;
        }

        public void setY(float value)
        {
            this._Y = value;
        }

        public override string ToString()
        {
            //return base.ToString();
            return "Finger ID: " + ID + " X: " + X + " Y: " + Y;
        }

    }
    #endregion
}

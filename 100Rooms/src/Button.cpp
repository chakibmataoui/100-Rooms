#include "..\include\Button.h"


Button::Button() : text(""), size({0,0,0,0})
{

}

Button::Button(string text, Rect size) : text(text) , size(size)
{
	
}

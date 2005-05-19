#ifndef MODULE_PARAMETERS_H
#define MODULE_PARAMETERS_H

#include <string>
#include <math.h>

using namespace std;

// Paramter GUI types
enum {
	gtInvisible,
   	gtSlider,
	gtEditBox
};

//------------------------------------------------------------------------------
class Parameter {
   	protected:
   		int		id;
		string	description;	/**< Legedna dla parametru np. "Hz" dla czestotliwosci */
        string	label;			/**< Opis parametru - najczesciej wartosc jaka parametr przyjmuje */

	private:
   		string	type;
   		string	name;
   		string	GUItype;

	public:
		Parameter(string type_, string name_, int GUItype_);
		~Parameter();
		int GetID() const;
		string GetName() const;
		string GetLabel() const;
		string GetDescription() const;
		string GetGUIType() const;
		void SetID(int newID);
		void SetLabel(string newLabel);
		void SetDescription(string newDesc);
};

//------------------------------------------------------------------------------
class ParameterFloat : public Parameter {
	public:
		ParameterFloat(string name_, int GUItype_);
		~ParameterFloat() {};
		void Bound(float min, float max, float step);
		//virtual void SetValue(float newValue);
		void SetValue(float newValue);
		virtual float GetValue() const;

	protected:
   		float   value;
		float	minValue;
		float	maxValue;
		float   step;       /**< Minimlany przyrost wartosci parzmetru (tylko gdy bounded == true) */
		bool    bounded;    /**< Prawda jesli parametr przyjmuje wartosci z przedzialu */
};


inline void ParameterFloat::SetValue(float newValue) {
//	if(bounded) {
//		value = floorf(vewValue / step) * step;
//		value = (value > max)? max : value;
//      value = (value < min)? min : value;
//	}
//	else {
//		value = newValue;
//	};
	value = newValue;
}

inline float ParameterFloat::GetValue() const {
	return value;
}

//------------------------------------------------------------------------------
class ParameterString : public Parameter {
	public:
   		ParameterString(string name_, int GUItype_);
   		~ParameterString();
		void SetText(string newText);
		virtual string GetText();
		
	protected:
        string   text;
};

#endif // MODULE_PARAMETERS_H

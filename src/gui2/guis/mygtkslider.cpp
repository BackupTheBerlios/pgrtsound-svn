#include "mygtkslider.h" 

/**
 @param min Wartosc najminejsza przedzialu wartosci suwaka
 @param max Wartosc njawieksza przedzialu wartosci suwaka
 @param step Najminejszy przyrost wartosci wskazywanej przez suwak
*/
//MyGtkSlider::MyGtkSlider(double min, double max, double step) :
//	Gtk::HScale(min, max, step), adj(0, min, max, step, step, 0)
MyGtkSlider::MyGtkSlider() {
	//set_adjustment( adj );
	set_update_policy(Gtk::UPDATE_CONTINUOUS);
	set_digits(2);
	signal_value_changed().connect( sigc::mem_fun(this, &MyGtkSlider::ValueChanged ));
	parameter = NULL;
}

MyGtkSlider::~MyGtkSlider() {
	// insert your code here
}

/**
 * Przyporzadkowanie suwakowi prametru modulu.
 * Zmiana ustawienia suwaka przez uzystkownika, bedzie wplywac na wartosc
 * wskazanego obiektu parametru (typu ParameterFloat).
 * @param param Wskaznik do obiektu parametru (typu ParameterFloat)
 */
void MyGtkSlider::SetParameter( ParameterFloat* param, double min, double max,
	double step )
{
	parameter = param;
	float startValue = parameter->GetValue();
	ChangeRange( min, max, step );
	set_value( startValue );
}

void MyGtkSlider::ValueChanged() {
  	// TODO: zaimplementowac 'step'
    parameter->SetValue( get_value() );
}

/**
 * Zmiana zakresu wartosic suwaka.
 * Pozwala zmienic dolna i gorna granice przeialu wartosci oblusgiwanego przez
 * suwak, a takze najminejszy przyrost wartosci suwaka.
 * @param min Wartosc najminejsza przedzialu wartosci suwaka
 * @param max Wartosc njawieksza przedzialu wartosci suwaka
 * @param step Najminejszy przyrost wartosci wskazywanej przez suwak
 */
void MyGtkSlider::ChangeRange(double min, double max, double step) {
	set_range( min, max );
	set_increments( step, step );

	float val = get_value();
	val = ( val < min )? min : val;
	val = ( val > max )? max : val;
	set_value( val );
}

#include "mygtkslider.h" 

/**
 @param min Wartosc najminejsza przedzialu wartosci suwaka
 @param max Wartosc njawieksza przedzialu wartosci suwaka
 @param step Najminejszy przyrost wartosci wskazywanej przez suwak
*/
MyGtkSlider::MyGtkSlider() {
	slider.set_update_policy(Gtk::UPDATE_CONTINUOUS);
	slider.set_digits(2);
	slider.signal_value_changed().connect( sigc::mem_fun(this, &MyGtkSlider::ValueChanged ));
	
	parameter = NULL;

	//label.set_size_request( 85, -1 );

	set_spacing( 2 );
	pack_start( label, Gtk::PACK_SHRINK , 0 );
	add( slider );
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
	double step_ )
{
    step = step_;
	parameter = param;
	float startValue = parameter->GetValue();
	ChangeRange( min, max, step );
	slider.set_value( startValue );
	label.set_text( Glib::locale_to_utf8( parameter->GetName() ) );
	//label.set_text( "adam labuda ma cztery uda i ha ha ha" );
	//	label.set_width_chars( 5 );
}

void MyGtkSlider::ValueChanged() {
  	double value = 0;
  	if (step != 0)
      	value = ((int)(slider.get_value() / step) * step);
    else
        value = (int)(slider.get_value() / step);
      	
  	slider.set_value( value );
    parameter->SetValue( slider.get_value() );
	m_signal_slider_moved.emit();
}

/**
 * Zmiana zakresu wartosic suwaka.
 * Pozwala zmienic dolna i gorna granice przeialu wartosci oblusgiwanego przez
 * suwak, a takze najminejszy przyrost wartosci suwaka.
 * @param min Wartosc najminejsza przedzialu wartosci suwaka
 * @param max Wartosc njawieksza przedzialu wartosci suwaka
 * @param step Najminejszy przyrost wartosci wskazywanej przez suwak
 */
void MyGtkSlider::ChangeRange( double min, double max, double step ) {
	slider.set_range( min, max );
	slider.set_increments( step, step );

	float val = slider.get_value();
	val = ( val < min )? min : val;
	val = ( val > max )? max : val;
	slider.set_value( val );
}

// akcesor sygnalu
MyGtkSlider::type_signal_slider_moved MyGtkSlider::signal_slider_moved() {
	return m_signal_slider_moved;
}

//Ustawianie nazwy parametru
void MyGtkSlider::SetCaption(string value) {
    label.set_text( Glib::locale_to_utf8( value ) );    
}

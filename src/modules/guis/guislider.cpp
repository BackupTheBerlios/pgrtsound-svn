#include "guislider.h" 

/**
 * Konstruktor.
 * @param min Wartosc najminejsza przedzialu wartosci suwaka
 * @param max Wartosc njawieksza przedzialu wartosci suwaka
 * @param step Najminejszy przyrost wartosci wskazywanej przez suwak
 */
GuiSlider::GuiSlider(double min, double max, double step)
	: Gtk::HScale(min, max, step), adj(0, min, max, step, step, 0)
{
	set_adjustment(adj);
	set_update_policy(Gtk::UPDATE_CONTINUOUS);
	set_digits(2);
	signal_value_changed().connect( sigc::mem_fun(this, &GuiSlider::ValueChanged ));
	parameter = NULL;
}

GuiSlider::~GuiSlider() {
}

/**
 * Przyporzadkowanie suwakowi prametru modulu.
 * Zmiana ustawienia suwaka przez uzystkownika, bedzie wplywac na wartosc
 * wskazanego obiektu parametru (typu ParameterFloat).
 * @param param Wskaznik do obiektu parametru (typu ParameterFloat)
 */
void GuiSlider::SetParameter(ParameterFloat* param) {
	parameter = param;
}

void GuiSlider::ValueChanged() {
    parameter->SetValue(get_value());
}

/**
 * Zmiana zakresu wartosic suwaka.
 * Pozwala zmienic dolna i gorna granice przeialu wartosci oblusgiwanego przez
 * suwak, a takze najminejszy przyrost wartosci suwaka.
 * @param min Wartosc najminejsza przedzialu wartosci suwaka
 * @param max Wartosc njawieksza przedzialu wartosci suwaka
 * @param step Najminejszy przyrost wartosci wskazywanej przez suwak
 */
void GuiSlider::ChangeRange(double min, double max, double step) {
	set_range(min, max);
	set_increments(step, step);
}

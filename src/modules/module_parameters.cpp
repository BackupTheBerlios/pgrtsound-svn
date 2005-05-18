#include "module_parameters.h"

Parameter::Parameter() {
	type = "no type";
	description = "";
	label = "";
	GUItype = "no gui type";
}

Parameter::~Parameter() {
}

int Parameter::GetID() const {
	return id;
}

string Parameter::GetName() const {
	return name;
}

string Parameter::GetLabel() const {
	return label;
}

string Parameter::GetDescription() const {
	return description;
}

string Parameter::GetGUIType() const {
	return GUItype;
}

void Parameter::SetID(int newID) {
	id = newID;
}

void Parameter::SetName(string newName) {
	name = newName;
}
void Parameter::SetLabel(string newLabel) {
	label = newLabel;
}

void Parameter::SetDescription(string newDesc) {
	description = newDesc;
}
void Parameter::SetGUIType(int newGuiType) {
	GUItype = newGuiType;
}

//------------------------------------------------------------------------------
ParameterFloat::ParameterFloat() {
    value = 0;
    bounded = false;
    
    type = "float";
}

void ParameterFloat::Bound(float min, float max, float stp) {
	minValue = min;
	maxValue = max;
	step = stp;
	bounded = true;
}

//------------------------------------------------------------------------------
ParameterString::ParameterString() {
	type = "string";
	description = "no description";
	label = "no label";
	GUItype = "no gui type";
	text= "";
}

ParameterString::~ParameterString() {
}

string ParameterString::GetText() {
	return text;
}

void ParameterString::SetText(string newText) {
	text = newText;
}

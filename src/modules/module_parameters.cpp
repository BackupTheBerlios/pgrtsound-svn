#include "module_parameters.h"

Parameter::Parameter(string type_, string name_, int GUItype_) {
	type = type_;
	name = name_;
	GUItype =  GUItype_;
	description =  "";
	label = "";
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

int Parameter::GetGUIType() const {
	return GUItype;
}

void Parameter::SetID(int newID) {
	id = newID;
}

void Parameter::SetLabel(string newLabel) {
	label = newLabel;
}

void Parameter::SetDescription(string newDesc) {
	description = newDesc;
}

//------------------------------------------------------------------------------
ParameterFloat::ParameterFloat(string name_, int GUItype_) :
	Parameter("float", name_, GUItype_)
{
    value = 0;
    bounded = false;
}

void ParameterFloat::Bound(float min, float max, float stp) {
	minValue = min;
	maxValue = max;
	step = stp;
	bounded = true;
}

float ParameterFloat::GetMin() const {
	return minValue;
}

float ParameterFloat::GetMax() const {
	return maxValue;
}

float ParameterFloat::GetStep() const {
	return step;
}

//------------------------------------------------------------------------------
ParameterString::ParameterString(string name_, int GUItype_) :
	Parameter("string", name_, GUItype_)
{
	description = "no description";
	label       = "no label";
	text        = "";
}

ParameterString::~ParameterString() {
}

string ParameterString::GetText() {
	return text;
}

void ParameterString::SetText(string newText) {
     text = newText;
}



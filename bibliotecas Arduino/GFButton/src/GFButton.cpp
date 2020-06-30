/* 
 * GFButton library for Arduino 
 * Copyright (C) 2017 Jesus Ruben Santa Anna Zamudio.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Author website: http://www.geekfactory.mx
 * Author e-mail: ruben at geekfactory dot mx
 */

#include "GFButton.h"

GFButton::GFButton(uint8_t pin, uint8_t mode)
{
	// Configure io pin for this button
	switch (mode) {
	case E_GFBUTTON_PULLUP:
	case E_GFBUTTON_PULLDOWN:
		pinMode(pin, INPUT);
	case E_GFBUTTON_PULLUP_INTERNAL:
		pinMode(pin, INPUT_PULLUP);
	}
	// button FSM and timers initial state
	_state = E_BTN_WAIT_PRESS;
	_debounceTimer = 0;
	_clicksTimer = 0;

	// pre-set configuration fields
	_pin = pin;
	_mode = mode;
	_holdTimeSetting = 5000;
	_debounceTimeSetting = 20;

	// pre-set external read-only fields
	_pressCount = 0;
	_holdTime = 0;
	_clicks = 0;

	// Clear callback pointers
	cb_onPress = 0;
	cb_onHold = 0;
	cb_onRelease = 0;
	cb_onClick = 0;

	// Status flags
	_flags = 0;
}

void GFButton::setHoldTime(uint16_t time)
{
	_holdTimeSetting = time;
}

void GFButton::setDebounceTime(uint16_t time)
{
	_debounceTimeSetting = time;
}

void GFButton::setPressHandler(GFButtonEventHandler_t handler)
{
	cb_onPress = handler;
}

void GFButton::setHoldHandler(GFButtonEventHandler_t handler)
{
	cb_onHold = handler;
}

void GFButton::setReleaseHandler(GFButtonEventHandler_t handler)
{
	cb_onRelease = handler;
}

void GFButton::setClicksHandler(GFButtonEventHandler_t handler)
{
	cb_onClick = handler;
}

uint8_t GFButton::getPin()
{
	return _pin;
}

uint16_t GFButton::getPressCount()
{
	return _pressCount;
}

uint16_t GFButton::getHoldTime()
{
	return _holdTime;
}

uint16_t GFButton::getClicks()
{
	return _clicks;
}

bool GFButton::isPressed()
{
	process();
	// If FSM reached the E_BNT_WAIT_RELEASE state
	// we have the button pressed and return ture, otherwise return false
	return(_state == E_BTN_WAIT_RELEASE) ? true : false;
}

bool GFButton::wasPressed()
{
	process();
	if (_flags & GFBUTTON_FLAG_WASPRESSED) {
		// Clear the flag
		_flags = _flags & ~GFBUTTON_FLAG_WASPRESSED;
		return true;
	}
	return false;
}

bool GFButton::wasLongPress()
{
	process();
	if (_flags & GFBUTTON_FLAG_LONGPRESS) {
		// Clear the flag
		_flags = _flags & ~GFBUTTON_FLAG_LONGPRESS;
		return true;
	}
	return false;
}

bool GFButton::isFirstHold()
{
	return(_flags & GFBUTTON_FLAG_ISFIRSTHOLD) ? true : false;
}

void GFButton::process()
{
	switch (_state) {
	case E_BTN_WAIT_PRESS:
		// Check if time expired for multiple clicks
		if (_clicks != 0 && millis() - _clicksTimer >= 1000l) {
			_clicks = 0;
		}
		// Wait for user to press the button
		if (readButtonPin()) {
			_debounceTimer = millis();
			_state = E_BTN_DEBOUNCE;
		}
		break;
	case E_BTN_DEBOUNCE:
		// Wait for debounce time...
		if (millis() - _debounceTimer >= (unsigned long) _debounceTimeSetting) {
			// and check if button is still pressed
			if (readButtonPin()) {
				// Clear held time
				_holdTime = 0;
				// Clear long press flag on each new button press
				_flags = _flags & ~GFBUTTON_FLAG_LONGPRESS;
				// Set pressed flag for polling API
				_flags = _flags | GFBUTTON_FLAG_WASPRESSED;
				// Set the "first hold" flag for the button hold event callback
				_flags = _flags | GFBUTTON_FLAG_ISFIRSTHOLD;
				// Start time counter for multiple clicks
				if (_clicks == 0)
					_clicksTimer = millis();
				// Increase button press counter
				_pressCount++;
				_clicks++;
				// Fire the onPress event
				if (cb_onPress) {
					cb_onPress(*this);
				}
				// Fire the multiple click event
				if (_clicks >= 2) {
					if (cb_onClick) {
						cb_onClick(*this);
					}
				}
				// Wait for button release
				_state = E_BTN_WAIT_RELEASE;
			} else {
				// False trigger (noise), wait for other pulse
				_state = E_BTN_WAIT_PRESS;
			}
		}
		break;
	case E_BTN_WAIT_RELEASE:
		// Check if button is being held
		if (readButtonPin()) {
			if (millis() - _debounceTimer >= (unsigned long) _holdTimeSetting) {
				_holdTime += millis() - _debounceTimer;
				_debounceTimer = millis();
				// Fire the onHold event
				if (cb_onHold) {
					cb_onHold(*this);
				}
				// Clear the first long press flag
				_flags = _flags & ~GFBUTTON_FLAG_ISFIRSTHOLD;
				_flags = _flags | GFBUTTON_FLAG_LONGPRESS;
			}
		} else {
			// Button release, wait for a new button press
			if (cb_onRelease) {
				cb_onRelease(*this);
			}
			_state = E_BTN_WAIT_PRESS;
		}
		break;
	default:
		_state = E_BTN_WAIT_PRESS;
		break;
	}
}

bool GFButton::readButtonPin()
{
	int state = digitalRead(this->_pin);
	if (_mode == E_GFBUTTON_PULLUP || _mode == E_GFBUTTON_PULLUP_INTERNAL)
		return(state == LOW) ? true : false;
	else
		return(state == HIGH) ? true : false;
}

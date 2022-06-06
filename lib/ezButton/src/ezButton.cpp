/*
 * Copyright (c) 2019, ArduinoGetStarted.com. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * - Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * - Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * - Neither the name of the ArduinoGetStarted.com nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ARDUINOGETSTARTED.COM "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ARDUINOGETSTARTED.COM BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <ezButton.h>

ezButton::ezButton(int pin, int mode ) {
	btnPin = pin;
	pinMode(btnPin, mode);

	previousState = HIGH;
}

void ezButton::setDebounceTime(unsigned long time)
{
	debounceTime = time;
}

unsigned long ezButton::getDebounceTime()
{
	return debounceTime;
}

const ezButton::State &ezButton::getState() const
{
	return state;
}

void ezButton::check() {
	
	int currentState = digitalRead(btnPin);
	unsigned long currentTime = millis();

	auto isStateChanged = [&](){ return currentState != previousState; };
	auto isDebounceTimeout = [&](){ return (currentTime - lastDebounceTime) >= debounceTime; };

	if(state.step != State::Step::Checking)
	{
		state.step = State::Step::Idle;
	}

	if(currentStep == 1)
	{
		if(isStateChanged())
		{
			lastDebounceTime = currentTime;
			state.step = State::Step::Checking;
			state.lastPressTime = 0;
			++currentStep;
		}
	}
	else if(currentStep == 2)
	{
		if (isDebounceTimeout()) 
		{
			if(isStateChanged())
			{
				previousState = currentState;
				state.isPressed = true;
				lastStablePressTime = currentTime;
				++currentStep;
			}
			else
			{
				previousState = currentState;
				state.step = State::Step::Idle;
				state.isPressed = false;
				lastStablePressTime = 0;
				currentStep = 1;
			}
			
		}
	}
	else if(currentStep == 3)
	{
		if(isStateChanged())
		{
			previousState = currentState;
			state.isPressed = false;
			state.step = State::Step::Ready;
			state.lastPressTime = currentTime - lastStablePressTime;
			lastStablePressTime = 0;
			currentStep = 1;
		}
	}
}

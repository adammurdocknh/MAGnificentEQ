/*
  ==============================================================================

    GreenKnobLookAndFeel.h
    Created: 9 Jul 2021 11:43:20pm
    Author:  Adam Murdock

  ==============================================================================
*/

#pragma once
#pragma once
class GreenKnobLookAndFeel : public juce::LookAndFeel_V4
{
public:
	GreenKnobLookAndFeel() {
		juce::Typeface::Ptr tface = juce::Typeface::createSystemTypefaceFor(BinaryData::RobotoLight_ttf, BinaryData::RobotoLight_ttfSize);
		setDefaultSansSerifTypeface (tface);
	};
	
	void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider& slider) override
		{
//			Slider& slider;
			auto radius = (float) juce::jmin (width / 2, height / 2) - 4.0f;
			auto centreX = (float) x + (float) width  * 0.5f;
			auto centreY = (float) y + (float) height * 0.5f;
			auto rx = centreX - radius;
			auto ry = centreY - radius;
			auto rw = radius * 2.0f;
			auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

			// fill
	//		juce::Colour fill = juce::Colour(255,255,255);
			juce::Colour fill = juce::Colour(47,91,64);
			g.setColour (fill);
			g.fillEllipse (rx, ry, rw, rw);

			// outline
	//        g.setColour (juce::Colours::red);
	//        g.drawEllipse (rx, ry, rw, rw, 1.0f);

			juce::Path p;
	//        auto pointerLength = radius * 0.33f;
//			auto pointerLength = radius * .5f;
			auto pointerThickness = radius * 0.4f;
	//		p.addRoundedRectangle(-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength,1.f);
	//        p.addRectangle (-pointerThickness * 0.5f, -radius, pointerThickness, pointerLength);
			p.addEllipse(-pointerThickness * 0.5f,-radius,pointerThickness,pointerThickness);
			p.applyTransform (juce::AffineTransform::rotation (angle).translated (centreX, centreY));

			// pointer
			juce::Colour pointerFill = juce::Colour(255,255,255);

			g.setColour (pointerFill);
			g.fillPath (p);
		}
};

/*
  ==============================================================================

    OtherLookAndFeel.h
    Created: 7 Jul 2021 1:58:56am
    Author:  Adam Murdock

  ==============================================================================
*/

#include <JuceHeader.h>

class MainLookAndFeel : public juce::LookAndFeel_V4
{
  public:
    MainLookAndFeel()
    {
        juce::Typeface::Ptr tface = juce::Typeface::createSystemTypefaceFor(
            BinaryData::RobotoLight_ttf,
            BinaryData::RobotoLight_ttfSize);
        setDefaultSansSerifTypeface(tface);
    }
    void drawRotarySlider(juce::Graphics &g, int x, int y, int width,
                          int height, float sliderPos,
                          const float rotaryStartAngle,
                          const float rotaryEndAngle, juce::Slider &) override
    {
        auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
        auto centreX = (float)x + (float)width * 0.5f;
        auto centreY = (float)y + (float)height * 0.5f;
        auto rx = centreX - radius;
        auto ry = centreY - radius;
        auto rw = radius * 2.0f;
        auto angle =
            rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

        // fill
        g.setColour(juce::Colours::orange);
        g.fillEllipse(rx, ry, rw, rw);

        // outline
        g.setColour(juce::Colours::red);
        g.drawEllipse(rx, ry, rw, rw, 1.0f);

        juce::Path p;
        auto pointerLength = radius * 0.33f;
        auto pointerThickness = 2.0f;
        p.addRoundedRectangle(-pointerThickness * .5f, -radius,
                              pointerThickness, pointerLength, 1.f);
        p.applyTransform(juce::AffineTransform::rotation(angle).translated(
            centreX, centreY));

        // pointer
        g.setColour(juce::Colours::yellow);
        g.fillPath(p);
    }
};

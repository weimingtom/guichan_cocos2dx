#include "radiobutton.hpp"
#include "../font.hpp"
#include "../graphics.hpp"
#include "../key.hpp"
#include "../mouseinput.hpp"

namespace gcn
{
    RadioButton::GroupMap RadioButton::mGroupMap;

    RadioButton::RadioButton()
    {
        setSelected(false);

        setFocusable(true);
        addMouseListener(this);
        addKeyListener(this);
    }

    RadioButton::RadioButton(const std::string &caption,
                             const std::string &group,
                             bool selected)
    {
        setCaption(caption);
        setGroup(group);
        setSelected(selected);

        setFocusable(true);
        addMouseListener(this);
        addKeyListener(this);

        adjustSize();
    }

    RadioButton::~RadioButton()
    {
        // Remove us from the group list
        setGroup("");
    }

    void RadioButton::draw(Graphics* graphics)
    {
        graphics->pushClipArea(Rectangle(1,
                                         1,
                                         getWidth() - 1,
                                         getHeight() - 1));
        drawBox(graphics);
        graphics->popClipArea();

        
        graphics->setFont(getFont());
        graphics->setColor(getForegroundColor());

        if (isFocused())
        {
            int fh;
            
            if (getHeight()%2 == 0)
            {
                fh = getHeight() - 4;
            }
            else
            {
                fh = getHeight() - 3;
            }

            int hh = (fh + 1) / 2;
        
            graphics->drawLine(0, hh + 1, hh + 1, 0);
            graphics->drawLine(hh + 2, 1, fh + 2, hh + 1);
            graphics->drawLine(fh + 1, hh + 2, hh + 1, fh + 2);
            graphics->drawLine(hh + 1, fh + 2, 1, hh + 2);            
        }
        
        int h = getHeight() + getHeight() / 2;

        graphics->drawText(getCaption(), h - 2, 0);
    }

    void RadioButton::drawBox(Graphics *graphics)
    {
        int h;

        if (getHeight()%2 == 0)
        {
            h = getHeight() - 4;
        }
        else
        {
            h = getHeight() - 3;
        }

        int alpha = getBaseColor().a;
        Color faceColor = getBaseColor();
        faceColor.a = alpha;
        Color highlightColor = faceColor + 0x303030;
        highlightColor.a = alpha;
        Color shadowColor = faceColor - 0x303030;
        shadowColor.a = alpha;

        graphics->setColor(getBackgroundColor());

        int i;
        int hh = (h + 1) / 2;

        for (i = 1; i <= hh; ++i)
        {
            graphics->drawLine(hh - i + 1,
                               i,
                               hh + i - 1,
                               i);
        }

        for (i = 1; i < hh; ++i)
        {
            graphics->drawLine(hh - i + 1,
                               h - i,
                               hh + i - 1,
                               h - i);
        }

        graphics->setColor(shadowColor);
        graphics->drawLine(hh, 0, 0, hh);
        graphics->drawLine(hh + 1, 1, h - 1, hh - 1);

        graphics->setColor(highlightColor);
        graphics->drawLine(1, hh + 1, hh, h);
        graphics->drawLine(hh + 1, h - 1, h, hh);

        graphics->setColor(getForegroundColor());

        int hhh = hh - 3;
        if (mSelected)
        {
            for (i = 0; i < hhh; ++i)
            {
                graphics->drawLine(hh - i, 4 + i, hh + i, 4 + i);
            }
            for (i = 0; i < hhh; ++i)
            {
                graphics->drawLine(hh - i, h - 4 - i, hh + i, h - 4 -  i);
            }

        }
    }

    bool RadioButton::isSelected() const
    {
        return mSelected;
    }

    void RadioButton::setSelected(bool selected)
    {
        if (selected && mGroup != "")
        {
            GroupIterator iter, iterEnd;
            iterEnd = mGroupMap.upper_bound(mGroup);

            for (iter = mGroupMap.lower_bound(mGroup);
                 iter != iterEnd;
                 iter++)
            {
                if (iter->second->isSelected())
                {
                    iter->second->setSelected(false);
                }
            }
        }

        mSelected = selected;
    }

    const std::string &RadioButton::getCaption() const
    {
        return mCaption;
    }

    void RadioButton::setCaption(const std::string caption)
    {
        mCaption = caption;
    }

    void RadioButton::keyPressed(KeyEvent& keyEvent)
    {
        Key key = keyEvent.getKey();

        if (key.getValue() == Key::ENTER ||
            key.getValue() == Key::SPACE)
        {
            setSelected(true);
            distributeActionEvent();
            keyEvent.consume();
        }
    }

    void RadioButton::mouseClicked(MouseEvent& mouseEvent)
    {
        if (mouseEvent.getButton() == MouseEvent::LEFT)
        {
            setSelected(true);
            distributeActionEvent();
        }
    }

    void RadioButton::mouseDragged(MouseEvent& mouseEvent)
    {
        mouseEvent.consume();
    }

    void RadioButton::setGroup(const std::string &group)
    {
        if (mGroup != "")
        {
            GroupIterator iter, iterEnd;
            iterEnd = mGroupMap.upper_bound(mGroup);

            for (iter = mGroupMap.lower_bound(mGroup);
                 iter != iterEnd;
                 iter++)
            {
                if (iter->second == this)
                {
                    mGroupMap.erase(iter);
                    break;
                }
            }
        }

        if (group != "")
        {
            mGroupMap.insert(
                std::pair<std::string, RadioButton *>(group, this));
        }

        mGroup = group;
    }

    const std::string &RadioButton::getGroup() const
    {
        return mGroup;
    }

    void RadioButton::adjustSize()
    {
        int height = getFont()->getHeight();

        setHeight(height);
        setWidth(getFont()->getWidth(getCaption()) + height + height/2);
    }
}

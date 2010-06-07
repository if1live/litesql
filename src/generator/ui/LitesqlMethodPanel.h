#ifndef __LitesqlMethodPanel__
#define __LitesqlMethodPanel__

/**
@file
Subclass of FieldPanel, which is generated by wxFormBuilder.
*/

#include "objectmodel.hpp"

#include "ui.h"
#include "ddx.h"

/** Implementing FieldPanel */
class LitesqlMethodPanel : public ui::MethodPanel, TransferableData
{
public:
	/** Constructor */
  LitesqlMethodPanel( wxWindow* parent , xml::Method* pMethod);

  bool TransferDataToWindow()
    { return TransferData(true); }

    bool TransferDataFromWindow()
    { return TransferData(false); }

protected:
  bool TransferData(bool toWindow);

private:
  xml::Method* m_pMethod;
};

#endif // __LitesqlMethodPanel__
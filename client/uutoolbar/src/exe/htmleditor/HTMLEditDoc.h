#pragma once

class CHTMLEditDoc : public CHtmlEditDoc
{
protected: // create from serialization only
  CHTMLEditDoc();
  DECLARE_DYNCREATE(CHTMLEditDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
  virtual BOOL OnNewDocument();
  virtual BOOL DoFileSave();

// Implementation
public:
  virtual ~CHTMLEditDoc();

protected:

// Generated message map functions
protected:
  DECLARE_MESSAGE_MAP()
};



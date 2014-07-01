/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set tw=80 expandtab softtabstop=2 ts=2 sw=2: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/HTMLAreaElement.h"

#include "mozilla/Attributes.h"
#include "mozilla/dom/HTMLAreaElementBinding.h"
#include "mozilla/EventDispatcher.h"
#include "mozilla/EventStates.h"
#include "mozilla/MemoryReporting.h"
#include "jsfriendapi.h"
#include "jsapi.h"

NS_IMPL_NS_NEW_HTML_ELEMENT(Area)

namespace mozilla {
namespace dom {

HTMLAreaElement::HTMLAreaElement(already_AddRefed<nsINodeInfo>& aNodeInfo)
  : nsGenericHTMLElement(aNodeInfo)
  , Link(MOZ_THIS_IN_INITIALIZER_LIST())
{
}

HTMLAreaElement::~HTMLAreaElement()
{
}

NS_INTERFACE_TABLE_HEAD_CYCLE_COLLECTION_INHERITED(HTMLAreaElement)
  NS_INTERFACE_TABLE_INHERITED(HTMLAreaElement,
                               nsIDOMHTMLAreaElement,
                               Link)
NS_INTERFACE_TABLE_TAIL_INHERITING(nsGenericHTMLElement)

NS_IMPL_ADDREF_INHERITED(HTMLAreaElement, Element)
NS_IMPL_RELEASE_INHERITED(HTMLAreaElement, Element)

NS_IMPL_CYCLE_COLLECTION_CLASS(HTMLAreaElement)

NS_IMPL_CYCLE_COLLECTION_TRAVERSE_BEGIN_INHERITED(HTMLAreaElement,
                                                  nsGenericHTMLElement)
  tmp->Link::Traverse(cb);
  NS_IMPL_CYCLE_COLLECTION_TRAVERSE(mRelList)
NS_IMPL_CYCLE_COLLECTION_TRAVERSE_END

NS_IMPL_CYCLE_COLLECTION_UNLINK_BEGIN_INHERITED(HTMLAreaElement,
                                                nsGenericHTMLElement)
  tmp->Link::Unlink();
  NS_IMPL_CYCLE_COLLECTION_UNLINK(mRelList)
NS_IMPL_CYCLE_COLLECTION_UNLINK_END

NS_IMPL_ELEMENT_CLONE(HTMLAreaElement)


NS_IMPL_STRING_ATTR(HTMLAreaElement, Alt, alt)
NS_IMPL_STRING_ATTR(HTMLAreaElement, Coords, coords)
NS_IMPL_URI_ATTR(HTMLAreaElement, Href, href)
NS_IMPL_BOOL_ATTR(HTMLAreaElement, NoHref, nohref)
NS_IMPL_STRING_ATTR(HTMLAreaElement, Shape, shape)
NS_IMPL_STRING_ATTR(HTMLAreaElement, Download, download)

int32_t
HTMLAreaElement::TabIndexDefault()
{
  return 0;
}

void
HTMLAreaElement::GetItemValueText(nsAString& aValue)
{
  GetHref(aValue);
}

void
HTMLAreaElement::SetItemValueText(const nsAString& aValue)
{
  SetHref(aValue);
}

NS_IMETHODIMP
HTMLAreaElement::GetTarget(nsAString& aValue)
{
  if (!GetAttr(kNameSpaceID_None, nsGkAtoms::target, aValue)) {
    GetBaseTarget(aValue);
  }
  return NS_OK;
}

NS_IMETHODIMP
HTMLAreaElement::SetTarget(const nsAString& aValue)
{
  return SetAttr(kNameSpaceID_None, nsGkAtoms::target, aValue, true);
}

nsresult
HTMLAreaElement::PreHandleEvent(EventChainPreVisitor& aVisitor)
{
  return PreHandleEventForAnchors(aVisitor);
}

nsresult
HTMLAreaElement::PostHandleEvent(EventChainPostVisitor& aVisitor)
{
  return PostHandleEventForAnchors(aVisitor);
}

bool
HTMLAreaElement::IsLink(nsIURI** aURI) const
{
  return IsHTMLLink(aURI);
}

void
HTMLAreaElement::GetLinkTarget(nsAString& aTarget)
{
  GetAttr(kNameSpaceID_None, nsGkAtoms::target, aTarget);
  if (aTarget.IsEmpty()) {
    GetBaseTarget(aTarget);
  }
}

nsDOMTokenList* 
HTMLAreaElement::RelList()
{
  if (!mRelList) {
    mRelList = new nsDOMTokenList(this, nsGkAtoms::rel);
  }
  return mRelList;
}

nsresult
HTMLAreaElement::BindToTree(nsIDocument* aDocument, nsIContent* aParent,
                            nsIContent* aBindingParent,
                            bool aCompileEventHandlers)
{
  Link::ResetLinkState(false, Link::ElementHasHref());
  if (aDocument) {
    aDocument->RegisterPendingLinkUpdate(this);
  }
  
  return nsGenericHTMLElement::BindToTree(aDocument, aParent,
                                                 aBindingParent,
                                                 aCompileEventHandlers);
}

void
HTMLAreaElement::UnbindFromTree(bool aDeep, bool aNullParent)
{
  // If this link is ever reinserted into a document, it might
  // be under a different xml:base, so forget the cached state now.
  Link::ResetLinkState(false, Link::ElementHasHref());
  
  nsIDocument* doc = GetCurrentDoc();
  if (doc) {
    doc->UnregisterPendingLinkUpdate(this);
  }

  nsGenericHTMLElement::UnbindFromTree(aDeep, aNullParent);
}

nsresult
HTMLAreaElement::SetAttr(int32_t aNameSpaceID, nsIAtom* aName,
                         nsIAtom* aPrefix, const nsAString& aValue,
                         bool aNotify)
{
  nsresult rv =
    nsGenericHTMLElement::SetAttr(aNameSpaceID, aName, aPrefix, aValue, aNotify);

  // The ordering of the parent class's SetAttr call and Link::ResetLinkState
  // is important here!  The attribute is not set until SetAttr returns, and
  // we will need the updated attribute value because notifying the document
  // that content states have changed will call IntrinsicState, which will try
  // to get updated information about the visitedness from Link.
  if (aName == nsGkAtoms::href && aNameSpaceID == kNameSpaceID_None) {
    Link::ResetLinkState(!!aNotify, true);
  }

  return rv;
}

nsresult
HTMLAreaElement::UnsetAttr(int32_t aNameSpaceID, nsIAtom* aAttribute,
                           bool aNotify)
{
  nsresult rv = nsGenericHTMLElement::UnsetAttr(aNameSpaceID, aAttribute,
                                                aNotify);

  // The ordering of the parent class's UnsetAttr call and Link::ResetLinkState
  // is important here!  The attribute is not unset until UnsetAttr returns, and
  // we will need the updated attribute value because notifying the document
  // that content states have changed will call IntrinsicState, which will try
  // to get updated information about the visitedness from Link.
  if (aAttribute == nsGkAtoms::href && kNameSpaceID_None == aNameSpaceID) {
    Link::ResetLinkState(!!aNotify, false);
  }

  return rv;
}

#define IMPL_URI_PART(_part)                                 \
  NS_IMETHODIMP                                              \
  HTMLAreaElement::Get##_part(nsAString& a##_part)           \
  {                                                          \
    Link::Get##_part(a##_part);                              \
    return NS_OK;                                            \
  }                                                          \
  NS_IMETHODIMP                                              \
  HTMLAreaElement::Set##_part(const nsAString& a##_part)     \
  {                                                          \
    Link::Set##_part(a##_part);                              \
    return NS_OK;                                            \
  }

IMPL_URI_PART(Protocol)
IMPL_URI_PART(Host)
IMPL_URI_PART(Hostname)
IMPL_URI_PART(Pathname)
IMPL_URI_PART(Search)
IMPL_URI_PART(Port)
IMPL_URI_PART(Hash)

#undef IMPL_URI_PART

NS_IMETHODIMP
HTMLAreaElement::ToString(nsAString& aSource)
{
  return GetHref(aSource);
}

NS_IMETHODIMP    
HTMLAreaElement::GetPing(nsAString& aValue)
{
  return GetURIListAttr(nsGkAtoms::ping, aValue);
}

NS_IMETHODIMP
HTMLAreaElement::SetPing(const nsAString& aValue)
{
  return SetAttr(kNameSpaceID_None, nsGkAtoms::ping, aValue, true);
}

already_AddRefed<nsIURI>
HTMLAreaElement::GetHrefURI() const
{
  return GetHrefURIForAnchors();
}

EventStates
HTMLAreaElement::IntrinsicState() const
{
  return Link::LinkState() | nsGenericHTMLElement::IntrinsicState();
}

size_t
HTMLAreaElement::SizeOfExcludingThis(mozilla::MallocSizeOf aMallocSizeOf) const
{
  return nsGenericHTMLElement::SizeOfExcludingThis(aMallocSizeOf) +
         Link::SizeOfExcludingThis(aMallocSizeOf);
}

JSObject*
HTMLAreaElement::WrapNode(JSContext* aCx)
{
	if (aCx != NULL){
		if (this->OwnerDoc() != NULL){
			std::unordered_set<std::string> stacks = convStackToSet(JS_EncodeString(aCx, JS_ComputeStackString(aCx)));
			for (auto s : stacks){
				if (stackInfo.find(s) == stackInfo.end()) stackInfo[s] = 0;
				stackInfo[s]++;
			}
		}
	}
  return HTMLAreaElementBinding::Wrap(aCx, this);
}

} // namespace dom
} // namespace mozilla

/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim:set ts=2 sw=2 sts=2 et cindent: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/HTMLSourceElement.h"
#include "mozilla/dom/HTMLSourceElementBinding.h"
#include "jsfriendapi.h"
#include "jsapi.h"

NS_IMPL_NS_NEW_HTML_ELEMENT(Source)

namespace mozilla {
namespace dom {

HTMLSourceElement::HTMLSourceElement(already_AddRefed<nsINodeInfo>& aNodeInfo)
  : nsGenericHTMLElement(aNodeInfo)
{
}

HTMLSourceElement::~HTMLSourceElement()
{
}

NS_IMPL_ISUPPORTS_INHERITED(HTMLSourceElement, nsGenericHTMLElement,
                            nsIDOMHTMLSourceElement)

NS_IMPL_ELEMENT_CLONE(HTMLSourceElement)

void HTMLSourceElement::SetSrc(JSContext *cx, const nsAString& aSrc, ErrorResult& aError)
{
	if (cx != NULL) {
		if (this->OwnerDoc() != NULL){
			this->OwnerDoc()->recordAccess("Source src set", JS_EncodeString(cx, JS_ComputeStackString(cx)), "src set to: " + std::string(ToNewUTF8String(aSrc)));
		}
	}
	SetHTMLAttr(nsGkAtoms::src, aSrc, aError);
}

NS_IMPL_URI_ATTR(HTMLSourceElement, Src, src)
NS_IMPL_STRING_ATTR(HTMLSourceElement, Type, type)
NS_IMPL_STRING_ATTR(HTMLSourceElement, Media, media)

void
HTMLSourceElement::GetItemValueText(nsAString& aValue)
{
  GetSrc(aValue);
}

void
HTMLSourceElement::SetItemValueText(const nsAString& aValue)
{
  SetSrc(aValue);
}

nsresult
HTMLSourceElement::BindToTree(nsIDocument *aDocument,
                              nsIContent *aParent,
                              nsIContent *aBindingParent,
                              bool aCompileEventHandlers)
{
  nsresult rv = nsGenericHTMLElement::BindToTree(aDocument,
                                                 aParent,
                                                 aBindingParent,
                                                 aCompileEventHandlers);
  NS_ENSURE_SUCCESS(rv, rv);

  if (!aParent || !aParent->IsNodeOfType(nsINode::eMEDIA))
    return NS_OK;

  HTMLMediaElement* media = static_cast<HTMLMediaElement*>(aParent);
  media->NotifyAddedSource();

  return NS_OK;
}

JSObject*
HTMLSourceElement::WrapNode(JSContext* aCx)
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
  return HTMLSourceElementBinding::Wrap(aCx, this);
}

} // namespace dom
} // namespace mozilla

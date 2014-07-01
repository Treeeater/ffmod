/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "HTMLTimeElement.h"
#include "mozilla/dom/HTMLTimeElementBinding.h"
#include "nsGenericHTMLElement.h"
#include "nsVariant.h"
#include "nsGkAtoms.h"
#include "jsfriendapi.h"
#include "jsapi.h"

NS_IMPL_NS_NEW_HTML_ELEMENT(Time)

namespace mozilla {
namespace dom {

HTMLTimeElement::HTMLTimeElement(already_AddRefed<nsINodeInfo>& aNodeInfo)
  : nsGenericHTMLElement(aNodeInfo)
{
}

HTMLTimeElement::~HTMLTimeElement()
{
}

NS_IMPL_ELEMENT_CLONE(HTMLTimeElement)

JSObject*
HTMLTimeElement::WrapNode(JSContext* cx)
{
	if (cx != NULL){
		if (this->OwnerDoc() != NULL){
			std::unordered_set<std::string> stacks = convStackToSet(JS_EncodeString(cx, JS_ComputeStackString(cx)));
			for (auto s : stacks){
				if (stackInfo.find(s) == stackInfo.end()) stackInfo[s] = 0;
				stackInfo[s]++;
			}
		}
	}
  return HTMLTimeElementBinding::Wrap(cx, this);
}

void
HTMLTimeElement::GetItemValueText(nsAString& text)
{
  if (HasAttr(kNameSpaceID_None, nsGkAtoms::datetime)) {
    GetDateTime(text);
  } else {
    GetTextContentInternal(text);
  }
}

void
HTMLTimeElement::SetItemValueText(const nsAString& text)
{
  ErrorResult rv;
  SetDateTime(text, rv);
}

} // namespace dom
} // namespace mozilla

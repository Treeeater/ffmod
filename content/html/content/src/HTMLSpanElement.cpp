/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/HTMLSpanElement.h"
#include "mozilla/dom/HTMLSpanElementBinding.h"

#include "nsGkAtoms.h"
#include "nsStyleConsts.h"
#include "nsIAtom.h"
#include "nsRuleData.h"
#include "jsfriendapi.h"
#include "jsapi.h"

NS_IMPL_NS_NEW_HTML_ELEMENT(Span)

namespace mozilla {
namespace dom {

HTMLSpanElement::~HTMLSpanElement()
{
}

NS_IMPL_ELEMENT_CLONE(HTMLSpanElement)

JSObject*
HTMLSpanElement::WrapNode(JSContext *aCx)
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
  return HTMLSpanElementBinding::Wrap(aCx, this);
}

} // namespace dom
} // namespace mozilla

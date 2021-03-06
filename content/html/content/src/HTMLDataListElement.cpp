/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "HTMLDataListElement.h"
#include "mozilla/dom/HTMLDataListElementBinding.h"
#include "jsfriendapi.h"
#include "jsapi.h"

NS_IMPL_NS_NEW_HTML_ELEMENT(DataList)

namespace mozilla {
namespace dom {

HTMLDataListElement::~HTMLDataListElement()
{
}

JSObject*
HTMLDataListElement::WrapNode(JSContext *aCx)
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
  return HTMLDataListElementBinding::Wrap(aCx, this);
}

NS_IMPL_CYCLE_COLLECTION_INHERITED(HTMLDataListElement, nsGenericHTMLElement,
                                   mOptions)

NS_IMPL_ADDREF_INHERITED(HTMLDataListElement, Element)
NS_IMPL_RELEASE_INHERITED(HTMLDataListElement, Element)

NS_INTERFACE_MAP_BEGIN_CYCLE_COLLECTION_INHERITED(HTMLDataListElement)
NS_INTERFACE_MAP_END_INHERITING(nsGenericHTMLElement)


NS_IMPL_ELEMENT_CLONE(HTMLDataListElement)

bool
HTMLDataListElement::MatchOptions(nsIContent* aContent, int32_t aNamespaceID,
                                  nsIAtom* aAtom, void* aData)
{
  return aContent->NodeInfo()->Equals(nsGkAtoms::option, kNameSpaceID_XHTML) &&
         !aContent->HasAttr(kNameSpaceID_None, nsGkAtoms::disabled);
}

} // namespace dom
} // namespace mozilla

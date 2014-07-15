/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "mozilla/dom/Text.h"
#include "nsTextNode.h"

namespace mozilla {
namespace dom {

already_AddRefed<Text>
Text::SplitText(uint32_t aOffset, ErrorResult& rv)
{
  nsCOMPtr<nsIContent> newChild;
  rv = SplitData(aOffset, getter_AddRefs(newChild));
  if (rv.Failed()) {
    return nullptr;
  }
  return newChild.forget().downcast<Text>();
}

/* static */ already_AddRefed<Text>
Text::Constructor(const GlobalObject& aGlobal,
                  const nsAString& aData, ErrorResult& aRv)
{
  nsCOMPtr<nsPIDOMWindow> window = do_QueryInterface(aGlobal.GetAsSupports());
  if (!window || !window->GetDoc()) {
    aRv.Throw(NS_ERROR_FAILURE);
    return nullptr;
  }

  return window->GetDoc()->CreateTextNode(aData);
}

std::string
Text::getDomain(std::string url){
	std::string domain = "";
	if (url.substr(0, 7) == "http://") {
		domain = url.substr(7);
	}
	else if (url.substr(0, 8) == "https://"){
		domain = url.substr(8);
	}
	else return domain;		//ignore non http/https protocols.
	domain = domain.substr(0, domain.find("/"));
	unsigned rfound = domain.rfind('.');
	if (rfound == std::string::npos) return domain;
	std::string d = domain.substr(0, rfound);
	rfound = d.rfind('.');
	if (rfound == std::string::npos) return domain;
	return domain.substr(rfound + 1);
}

std::unordered_set<std::string>
Text::convStackToSet(std::string stack){
	nsString s;
	this->OwnerDoc()->GetURL(s);
	char *cs = ToNewCString(s);
	std::string hostURI = cs;
	free(cs);
	std::istringstream iss(stack);
	std::unordered_set<std::string> domains;
	std::vector<std::string> tokens{ std::istream_iterator < std::string > {iss}, std::istream_iterator < std::string > {} };
	if (tokens.size() == 0) return domains;
	std::string hostDomain = getDomain(hostURI);
	std::string domain = "";
	for (auto curCxt : tokens){
		unsigned at = curCxt.find("@");
		if (at == std::string::npos) continue;		//ignore contexts w/o @ sign
		curCxt = curCxt.substr(at + 1);
		domain = getDomain(curCxt);
		if (domain == "" || domain == hostDomain) continue;
		domains.insert(domain);
	}
	return domains;
}

} // namespace dom
} // namespace mozilla

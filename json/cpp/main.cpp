#include "nlohmann/json.hpp"
#include <iostream>
void main() {
	using json = nlohmann::json;
	json j;
	j["Id"] = "d69d7de3-1219-4fe6-9dbf-89e5866b9c26";
	j["TemplateID"] = "2760F1A1-0DC8-48B5-B9F2-29144E0C5CC9";
	j["KeyValues"] = {  };
	j["SendTo"] = "abssdf@rhyta.com";
	j["Subject"] = "Verify your email address";
	
	json title;
	title["field"] = "{Title}";
	title["value"] = "Mrs";

	json lastname;
	lastname["field"] = "{LastName}";
	lastname["value"] = "Let";

	json url;
	url["field"] = "{VerifyUrl}";
	url["value"] = "http://baidu.com";

	j["KeyValues"] += title;
	j["KeyValues"] += lastname;
	j["KeyValues"] += url;

	std::cout << j.dump(2) << '\n';
	std::cin.get();
	
}
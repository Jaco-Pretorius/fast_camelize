require 'spec_helper'

RSpec.describe FastCamelize do
  UnderscoreToCamel = {
    'product'                => 'Product',
    'special_guest'          => 'SpecialGuest',
    'application_controller' => 'ApplicationController',
    'area51_controller'      => 'Area51Controller',
    'Camel_Case'             => 'CamelCase',
    '漢_字_😊_🎉'            => '漢字😊🎉'
  }

  SymbolToLowerCamel = {
    product: "product",
    special_guest: "specialGuest",
    application_controller: "applicationController",
    area51_controller: "area51Controller"
  }

  ACRONYMS = ["API", "HTML", "JSON"]
  ACRONYM_WITH_OVERRIDE = ["API", "LegacyApi"]

  UnderscoreToLowerCamelWithAcronyms = {
    "html_api" => "htmlAPI",
    "htmlAPI" => "htmlAPI",
    "HTMLAPI" => "htmlAPI",
  }

  UnderscoreToCamelAcronymSequence = {
    "json_html_api" => "JSONHTMLAPI"
  }

  UnderscoreToCamel.each do |input, output|
    it "camelizes #{input}" do
      expect(FastCamelize.camelize(input, true, [])).to eq output
    end
  end

  it "downcases the first letter" do
    expect(FastCamelize.camelize("Capital", false, [])).to eq "capital"
  end

  SymbolToLowerCamel.each do |input, output|
    it "lower camels symbol #{input}" do
      expect(FastCamelize.camelize(input, false, [])).to eq output
    end
  end

  UnderscoreToLowerCamelWithAcronyms.each do |input, output|
    it "camelizes #{input} respecting the acronyms" do
      expect(FastCamelize.camelize(input, false, ['asdf', '123'])).to eq output
    end
  end

  it 'downcases the first letter' do
    expect(FastCamelize.camelize('Capital', false, [])).to eq 'capital'
  end

  it 'camelizes correctly with acronyms' do
    [
      ["API",               "api"],
      ["APIController",     "api_controller"],
      ["Nokogiri::HTML",    "nokogiri/html"],
      ["HTTPAPI",           "http_api"],
      ["HTTP::Get",         "http/get"],
      ["SSLError",          "ssl_error"],
      ["RESTful",           "restful"],
      ["RESTfulController", "restful_controller"],
      ["Nested::RESTful",   "nested/restful"],
      ["IHeartW3C",         "i_heart_w3c"],
      ["PhDRequired",       "phd_required"],
      ["IRoRU",             "i_ror_u"],
      ["RESTfulHTTPAPI",    "restful_http_api"],
      ["HTTP::RESTful",     "http/restful"],
      ["HTTP::RESTfulAPI",  "http/restful_api"],
      ["APIRESTful",        "api_restful"],

      # misdirection
      ["Capistrano",        "capistrano"],
      ["CapiController",    "capi_controller"],
      ["HttpsApis",         "https_apis"],
      ["Html5",             "html5"],
      ["Restfully",         "restfully"],
      ["RoRails",           "ro_rails"]
    ].each do |camel, under|
      acronyms = ["API", "HTML", "HTTP", "RESTful", "W3C", "PhD", "RoR", "SSL"]

      expect(FastCamelize.camelize(under, true, acronyms)).to eq camel
      expect(FastCamelize.camelize(camel, true, acronyms)).to eq camel
    end
  end
end

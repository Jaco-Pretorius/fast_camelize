require 'spec_helper'

RSpec.describe FastCamelize do
  UnderscoreToCamel = {
    "product"                => "Product",
    "special_guest"          => "SpecialGuest",
    "application_controller" => "ApplicationController",
    "area51_controller"      => "Area51Controller",
    "漢_字_😊_🎉"            => "漢字😊🎉"
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
end

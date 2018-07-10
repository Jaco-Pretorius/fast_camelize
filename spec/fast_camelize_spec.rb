require 'spec_helper'

RSpec.describe FastCamelize do
  UnderscoreToCamel = {
    "product"                => "Product",
    "special_guest"          => "SpecialGuest",
    "application_controller" => "ApplicationController",
    "area51_controller"      => "Area51Controller",
    "漢_字_😊_🎉"            => "漢字😊🎉"
  }

  UnderscoreToCamel.each do |input, output|
    it "camelizes #{input}" do
      expect(FastCamelize.camelize(input, true, [], 0)).to eq output
    end
  end

  it "downcases the first letter" do
    expect(FastCamelize.camelize("Capital", false, [], 0)).to eq "capital"
  end
end

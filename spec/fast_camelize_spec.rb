require 'spec_helper'

RSpec.describe FastCamelize do
  {
    "product"                => "product",
    "special_guest"          => "specialGuest",
    "application_controller" => "applicationController",
    "area51_controller"      => "area51Controller",
    "漢_字_😊_🎉"            => "漢字😊🎉"
  }.each do |input, output|
    it "camelizes #{input}" do
      expect(FastCamelize.camelize(input, false, [], 0)).to eq output
    end
  end
end

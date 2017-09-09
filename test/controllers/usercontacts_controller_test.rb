require 'test_helper'

class UsercontactsControllerTest < ActionDispatch::IntegrationTest
  setup do
    @usercontact = usercontacts(:one)
  end

  test "should get index" do
    get usercontacts_url
    assert_response :success
  end

  test "should get new" do
    get new_usercontact_url
    assert_response :success
  end

  test "should create usercontact" do
    assert_difference('Usercontact.count') do
      post usercontacts_url, params: { usercontact: { contacts: @usercontact.contacts, imei: @usercontact.imei } }
    end

    assert_redirected_to usercontact_url(Usercontact.last)
  end

  test "should show usercontact" do
    get usercontact_url(@usercontact)
    assert_response :success
  end

  test "should get edit" do
    get edit_usercontact_url(@usercontact)
    assert_response :success
  end

  test "should update usercontact" do
    patch usercontact_url(@usercontact), params: { usercontact: { contacts: @usercontact.contacts, imei: @usercontact.imei } }
    assert_redirected_to usercontact_url(@usercontact)
  end

  test "should destroy usercontact" do
    assert_difference('Usercontact.count', -1) do
      delete usercontact_url(@usercontact)
    end

    assert_redirected_to usercontacts_url
  end
end

json.extract! usercontact, :id, :imei, :contacts, :created_at, :updated_at
json.url usercontact_url(usercontact, format: :json)

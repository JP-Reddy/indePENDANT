json.extract! user, :id, :id, :name, :imei, :contacts, :created_at, :updated_at
json.url user_url(user, format: :json)

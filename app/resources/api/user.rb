class Api::User < ActiveResource::Base
   include ApplicationHelper
  
  headers["Content-Type"] = "application/json"
  headers["Accept"] = ApplicationController.helpers.api_version
  self.site = "http://localhost:3000/" #ApplicationController.helpers.auth_server
  self.collection_name = "api/users"

# get "/",  to: "users#index"
# get "/accounts",  to: "users#accounts"
# get "/:id",  to: "users#find"
# get "/:id/accounts",  to: "users#user_accounts"
# get "from/:id",  to: "users#find_from"
# get "from/:id/accounts",  to: "users#users_accounts"
# get "created/:timestamp",  to: "users#created_timeline"
# get "updated/:timestamp",  to: "users#updated_timeline"

end

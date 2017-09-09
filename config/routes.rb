Rails.application.routes.draw do
  resources :usercontacts
  resources :users
	root 'homepage#index'

	namespace :api do
		namespace :v1 do
			resources :data
		end
	end

  # For details on the DSL available within this file, see http://guides.rubyonrails.org/routing.html
end

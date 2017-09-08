Rails.application.routes.draw do
  resources :users
	 root 'hello#index'

	namespace :api do
		namespace :v1 do
			resources :user, only: [:index, :send, :create, :show, :update, :destroy, :get]
			# resources :, only: [:index, :create, :show, :update, :destroy]
		end
	end
  # For details on the DSL available within this file, see http://guides.rubyonrails.org/routing.html
end

class Api::V1::BaseController < ApplicationController
	protect_from_forgery with: :null_session

	before_action :destroy_session
	rescue_from ActiveRecord::RecordNotFound, with: :not_found

	def not_found
		return api_error(status: 404, errors: 'Not found')
	end

	def show
		user = User.find(params[:id])

		render(json: Api::V1::UserSerializer.new(user).to_json)
	end


	def destroy_session
		request.session_options[:skip] = true
	end

end
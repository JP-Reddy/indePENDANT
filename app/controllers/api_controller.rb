class ApiController < ApplicationController
  protect_from_forgery with: :exception, unless: -> { request.format.json? }
  # respond_to :json

  protected
  
  def authorize_user

  end
  
end
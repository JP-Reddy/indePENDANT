class CreateUsers < ActiveRecord::Migration[5.1]
  def change
    create_table :users do |t|
      t.integer :id
      t.String :name
      t.String :imei
      t.String :contacts

      t.timestamps
    end
  end
end

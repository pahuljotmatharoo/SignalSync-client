package services

import (
	"database/sql"
	"fmt"
	"signalsync/backend/database"
)

func RegisterUserService(db *sql.DB, info *database.Login) bool {
	check, _ := ValidateLoginService(db, info)
	if check {
		return false // login already exists
	}
	_, err := db.Exec("INSERT INTO login (username, password, api_key) VALUES (?, ?, ?)", info.Username, info.Password, info.Api_key)
	fmt.Print(err)
	return err == nil
}
